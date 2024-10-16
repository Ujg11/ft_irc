/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 17:46:18 by agrimald          #+#    #+#             */
/*   Updated: 2024/10/16 12:51:57 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/commands/Join.hpp"

void Join::createNewChannel(Client &user, Server &server, const std::string &channelName, const std::string &password)
{
	server.create_channel(channelName, password, user);
	
	std::string joinMessage = ":" + user.getNickname() + " JOIN " + channelName + "\r\n";
	server.message.sendMessage(user, joinMessage);
	std::string topicMessage = ":" + server.getServerName() + " 331 " + user.getNickname() + " " + channelName + " :No topic is set\r\n";
	server.message.sendMessage(user, topicMessage);
}

void Join::handleExistingChannel(Client &user, Server &server, Channel *channel, const std::string &channelName)
{
	if (channel->isFull())
	{
		std::string cmd = channelName;
		server.message.sendMessage(user, server.message.getMessage(471, user, cmd));
		return ;
	}
	if (channel->isInvitedOnly() && !channel->isInvited(user))
	{
		std::string cmd = channelName;
		server.message.sendMessage(user, server.message.getMessage(473, user, cmd));
		return ;
	}
	channel->removeInvitedClient(user.getNickname());
	channel->addClient(user);
	std::string joinMessage = ":" + user.getNickname() + " JOIN " + channelName + "\r\n";
	channel->broadcastMessage(joinMessage, user);
	server.message.sendMessage(user, joinMessage);
	// Enviamos la lista de usuarios al cliente
	/*std::string userList = ":" + server.getServerName() + " 353 " + user.getNickname() + " = " + channelName + " :" + channel->getUserList() + "\r\n";
	server.message.sendMessage(user, userList);
	std::string endUserList = ":" + server.getServerName() + " 366 " + user.getNickname() + " " + channelName + " :End of /NAMES list\r\n";
	server.message.sendMessage(user, endUserList);*/
	// Si hay, enviamos el topic al cliente que se acaba de unir
	if (!channel->getTopic().empty())
	{
		std::string topicMessage = ":" + server.getServerName() + " 332 " + user.getNickname() + " " + channelName + " :" + channel->getTopic() + "\r\n";
		server.message.sendMessage(user, topicMessage);
	}
	else
	{
		std::string topicMessage = ":" + server.getServerName() + " 331 " + user.getNickname() + " " + channelName + " :No topic is set\r\n";
		server.message.sendMessage(user, topicMessage);
	}
}

bool Join::handleJoinChannel(Client &user, Server &server, const std::string &channelName, const std::string &password)
{
	if (channelName.empty() || channelName[0] != '#')
	{
		server.message.sendMessage(user, "El nombre del canal debe empezar con #\r\n");
		return (false);
	}
	Channel *channel = server.getChannel(channelName);
	if (channel)
	{
		if (!channel->getKey().empty())
		{
			if (password.empty())
			{
				std::string cmd = channelName;
				server.message.sendMessage(user, server.message.getMessage(475, user, cmd));
				return (false);
			}
			if (channel->getKey() != password)
			{
				std::string cmd = channelName;
				server.message.sendMessage(user, server.message.getMessage(475, user, cmd));
				return (false);
			}
		}
		handleExistingChannel(user, server, channel, channelName);
	}
	else
		createNewChannel(user, server, channelName, password);
	return (true);
}

void Join::execute(Server &server, Client &c, std::vector<std::string> args)
{
	if (args.size() < 1)
	{
		std::string cmd = "JOIN";
		server.message.sendMessage(c, server.message.getMessage(461, c, cmd));
		return ;
	}
	std::vector<std::string> channels = ft_split(args.at(0), ",");
	std::vector<std::string> passwords;
	if (args.size() > 1)
		passwords = ft_split(args.at(1), ",");
	for (size_t i = 0; i < channels.size(); i++)
	{
		std::string channelName = channels.at(i);
		std::string password = "";
		if (i < passwords.size())
			password = passwords.at(i);
		handleJoinChannel(c, server, channelName, password);
	}
}
