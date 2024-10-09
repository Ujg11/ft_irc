/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 14:21:42 by ojimenez          #+#    #+#             */
/*   Updated: 2024/10/09 18:08:16 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/commands/Part.hpp"

void Part::execute(Server &server, Client &c, std::vector<std::string> args)
{
	if (args.empty())
	{
		std::string cmd = "PART";
		server.message.sendMessage(c, server.message.getMessage(461, c, cmd));
		return ;
	}
	std::vector<std::string> channelsNames = ft_split(args[0], ",");
	std::string message = "";
	if (args.size() > 1)
	{
		for (size_t i = 1; i < args.size(); i++)
		   message += args[i] + " ";
		if (!message.empty() && message[message.length() - 1] == ' ')
			message.erase(message.length() - 1);
	}
	for (size_t i = 0; i < channelsNames.size(); ++i)
	{
		std::string channelName = channelsNames[i];
		Channel *channel = server.getChannel(channelName);
		if (channel == NULL)
		{
			server.message.sendMessage(c, server.message.getMessage(403, c));
			continue ;
		}
		if (!channel->isClient(c))
		{
			server.message.sendMessage(c, server.message.getMessage(442, c));
			continue ;
		}
		std::string partMessage = ":" + c.getNickname() + " PART " + channelName;
		if (!message.empty())
			partMessage += " :" + message;
		partMessage += "\r\n";
		channel->broadcastMessage(partMessage, c);
		channel->removeClient(c);
		if (channel->isOperator(c))
			channel->removeOperator(c);
		c.removeJoinedChannel(channelName);
		if (channel->isEmpty())
			server.deleteChannel(channelName);
	}
}

