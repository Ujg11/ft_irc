/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 18:20:14 by ojimenez          #+#    #+#             */
/*   Updated: 2024/09/20 15:09:39 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/commands/Nick.hpp"

static int updateNick(Server &s, Client &c, std::vector<std::string> args)
{
	std::string newNick = args.at(0);
	if (s.findClient(newNick) != NULL)
	{
		s.message.sendMessage(c, s.message.getMessage(433, c));
		return (-1);
	}
	std::vector<std::string> channels = c.getJoinedChannels();
	std::string oldNick = c.getNickname();
	c.setNickname(newNick);
	std::string message = ":" + oldNick + "!" + c.getUsername() + "@" + c.getIp() + "NICK " + newNick + "\r\n";
	for (size_t i = 0; i < channels.size(); i++)
	{
		Channel *channel = s.getChannel(channels[i]);
		if (channel != NULL)
		{
			channel->broadcastMessage(message, c);
		}
	}
	return (0);
}

void Nick::execute(Server &server, Client &c, std::vector<std::string> args)
{
	if (args.empty() || args.at(0).empty())
	{
		server.message.sendMessage(c, server.message.getMessage(431, c));
        return ;
    }
	if (!c.getNickname().empty() && server.findClient(c.getNickname()) != NULL)
		updateNick(server, c, args);
	else if (c.getNickname().empty())
	{
		c.setNickname(args.at(0));
		c.hasNick = true;
	}
}
