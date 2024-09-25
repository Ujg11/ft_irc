/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 14:21:42 by ojimenez          #+#    #+#             */
/*   Updated: 2024/09/25 15:08:49 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/commands/Part.hpp"

void Part::execute(Server &server, Client &c, std::vector<std::string> args)
{
    if (args.size() < 1)
    {
        std::string cmd = "PART";
        server.message.sendMessage(c, server.message.getMessage(461, c, cmd));
        return ;
    }
    std::string channelName = args.at(0);
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
    std::string partMessage = ":" + c.getNickname() + " PART " + channelName + "\r\n";
    channel->broadcastMessage(partMessage, c);
    channel->removeClient(c);
    if (channel->isOperator(c))
        channel->removeOperator(c);
    c.removeJoinedChannel(channelName);
    if (channel->isEmpty())
        server.deleteChannel(channelName);
}

