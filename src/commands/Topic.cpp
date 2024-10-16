/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 15:05:40 by ojimenez          #+#    #+#             */
/*   Updated: 2024/10/16 12:39:44 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/commands/Topic.hpp"

void Topic::execute(Server &server, Client &c, std::vector<std::string> args)
{
	if (args.size() < 1)
	{
		server.message.sendMessage(c, server.message.getMessage(403, c));
		return ;
	}
	std::string channelName = args.at(0);
	Channel *channel = server.getChannel(channelName);
	if (channel == NULL)
	{
		server.message.sendMessage(c, server.message.getMessage(403, c));
		return ;
	}
	if (args.size() == 1)
	{
		if (!channel->getTopic().empty())
		{
			std::string topic = ":" + server.getServerName() + " 332 " + c.getNickname() + " " + channelName + " :" + channel->getTopic() + "\r\n"; 
			send(c.getFd(), topic.c_str(), topic.length(), 0);
		}
		else
		{
			std::string topicErr = ":" + server.getServerName() + " 331 " + c.getNickname() + " " + channelName + " :No topic is set\r\n";
			send(c.getFd(), topicErr.c_str(), topicErr.length(), 0);
		}
		return ;
	}
	if (channel->getTopicRestricted() && !channel->isOperator(c))
	{
		server.message.sendMessage(c, server.message.getMessage(482, c, channelName));
		return ;
	}
	std::string newTopic = "";
	for (size_t i = 1; i < args.size(); i++)
		newTopic += args[i] + " ";
	if (!newTopic.empty() && newTopic[newTopic.length() - 1] == ' ')
		newTopic.erase(newTopic.length() - 1);
	channel->setTopic(newTopic);
	//Notificamos a todos los integrantes del canal sobre el nuevo tema
	std::string notification = ":" + c.getPrefix() + " TOPIC " + channelName + " :" + newTopic + "\r\n";
	channel->broadcast(notification);
}