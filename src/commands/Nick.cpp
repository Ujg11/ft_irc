/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 18:20:14 by ojimenez          #+#    #+#             */
/*   Updated: 2024/09/18 18:50:18 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/commands/Nick.hpp"

static int updateNick(Server &s, Client &c, std::vector<std::string> args)
{
	if (args.empty()) {
        std::string reason = "Error: No nickname provided.\r\n";
        send(c.getFd(), reason.c_str(), reason.length(), 0);
        return -1;
    }
	std::string newNick = args.at(0);
	if (!newNick.empty() && s.findClient(c.getNickname()))
	{
		if (s.findClient(args.at(0)) == NULL)
		{
			std::cout << "UpdateNick, nou nick: " << args.at(0) << std::endl;
			c.setNickname(args.at(0));
		}
		else
		{
			std::string reason = c.getNickname() + " :Nickname is already in use" + "\r\n";
			send(c.getFd(), reason.c_str(), reason.length(), 0);
			//:server_name 433 <nickname> <new_nick> :Nickname is already in use
			
		}
			
	}
	return (0);
}

void Nick::execute(Server &server, Client &c, std::vector<std::string> args)
{
	
}
