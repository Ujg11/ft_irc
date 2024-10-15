/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 18:13:00 by ojimenez          #+#    #+#             */
/*   Updated: 2024/10/15 18:28:44 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/commands/Who.hpp"

void Who::execute(Server &server, Client &c, std::vector<std::string> args)
{
	if (args.size() < 1)
	{
		std::string cmd = "WHO";
		server.message.sendMessage(c, server.message.getMessage(461, c, cmd));
		return ;
	}
	if (args.at(0).empty() || args.at(0)[0] != '#')
	{
		server.message.sendMessage(c, "El nombre del canal debe empezar con #\r\n");
		return ;
	}
	// Enviamos la lista de usuarios al cliente
	std::string channelName = args.at(0);
	Channel *channel = server.getChannel(channelName);
	if (channel)
	{
		 std::string userList = ":" + server.getServerName() + " 353 " + c.getNickname() + " = " + channelName + " :" + channel->getUserList() + "\r\n";
		server.message.sendMessage(c, userList);
		std::string endUserList = ":" + server.getServerName() + " 366 " + c.getNickname() + " " + channelName + " :End of /NAMES list\r\n";
		server.message.sendMessage(c, endUserList);
	}
	else
		server.message.sendMessage(c, "Canal no encontrado.\r\n");
}
