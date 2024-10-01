/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 18:20:14 by ojimenez          #+#    #+#             */
/*   Updated: 2024/10/01 15:25:39 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/commands/Nick.hpp"

static bool isNickValid(const std::string n)
{
	if (n.empty() || isdigit(n[0]))
		return (false);
	for (size_t i = 0; i < n.length(); i++)
	{
		char c = n[i];
		if (!isalnum(c) && c != '_' && c != '-' && c != '^')
			return (false);
	}
	return (true);
}

static void updateNick(Server &s, Client &c, std::vector<std::string> args)
{
	std::string newNick = args.at(0);
	if (!isNickValid(newNick))
	{
		s.message.sendMessage(c, s.message.getMessage(432, c));
		return ;
	}
	if (s.findClient(newNick) != NULL)
	{
		s.message.sendMessage(c, s.message.getMessage(433, c));
		return ;
	}
	std::string oldNick = c.getNickname();
	c.setNickname(newNick);
	std::vector<Client> clients = s.getClients();
	std::string message = ":" + oldNick + " NICK " + newNick + "\r\n";
	for (size_t i = 0; i < clients.size(); i++)
	{
		s.message.sendMessage(clients[i], message);
	}
}

void Nick::execute(Server &server, Client &c, std::vector<std::string> args)
{
	if (args.empty() || args.at(0).empty())
	{
		server.message.sendMessage(c, server.message.getMessage(431, c));
        return ;
    }
	std::string newNick = args.at(0);
	if (!c.getNickname().empty())
		updateNick(server, c, args);
	else
	{
		if (server.findClient(newNick) != NULL)
		{
			server.message.sendMessage(c, server.message.getMessage(433, c));
			return ;
		}
		if (isNickValid(newNick))
		{
			c.setNickname(args.at(0));
			c.hasNick = true;
		}
		else
			server.message.sendMessage(c, server.message.getMessage(432, c));
	}
}
