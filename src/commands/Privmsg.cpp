/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 11:40:29 by ojimenez          #+#    #+#             */
/*   Updated: 2024/09/21 16:20:51 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/commands/Privmsg.hpp"

void Privmsg::execute(Server &server, Client &c, std::vector<std::string> args)
{
    if (args.empty() || args.at(0).empty())
    {
        server.message.sendMessage(c, server.message.getMessage(411, c));
        return ;
    }
    if (args.size() < 2 || (args.size() >= 2 && args.at(1).empty()))
    {
        server.message.sendMessage(c, server.message.getMessage(412, c));
        return ;
    }
    std::string recieverNick = args.at(0);
    std::string message = "";
    for (size_t i = 1; i < args.size() - 1; i++)
        message += args.at(i) + " ";
    message += "\r\n";
    if (message[0] == ':')
        message.erase(0, 1);
    if (recieverNick[0] == '#')
        server.handleChannelMessage(c, recieverNick, message);
    else
        server.handlePrivMessag(c, recieverNick, message);
}

