/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 11:16:36 by ojimenez          #+#    #+#             */
/*   Updated: 2024/10/09 18:03:20 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/commands/Invite.hpp"

void Invite::execute(Server &server, Client &c, std::vector<std::string> args)
{
	if (args.size() < 2 || args[0].empty() || args[1].empty())
	{
		std::string cmd = "INVITE";
		server.message.sendMessage(c, server.message.getMessage(461, c, cmd));
		return ;
	}
	std::string target = args.at(0);
	std::string channelName = args.at(1);
	Channel *channel = server.getChannel(channelName);
	if (channel == NULL)
	{
		server.message.sendMessage(c, server.message.getMessage(403, c));
		return ;
	}
	if (!channel->isClient(c))
	{
		server.message.sendMessage(c, server.message.getMessage(442, c));
		return ;
	}
	Client *clientInvited = server.findClient(target);
	if (clientInvited == NULL)
	{
		server.message.sendMessage(c, server.message.getMessage(401, c));
		return ;
	}
	if (channel->isClient(*clientInvited))
	{
		std::string chan = channel->getName();
		server.message.sendMessage(c, server.message.getMessage(443, c, chan));
		return ;
	}
	//Si el canal esta en solo invitacion debes de ser operador
	if (channel->isInvitedOnly() && !channel->isOperator(c))
	{
		server.message.sendMessage(c, server.message.getMessage(482, c));
		return ;
	}
	channel->addInvitedClient(*clientInvited);
	std::string msg = ":" + c.getNickname() + " INVITE " + target + " :" + channelName + "\r\n";
	server.message.sendMessage(c, msg);
	server.message.sendMessage(*clientInvited, msg);
}

