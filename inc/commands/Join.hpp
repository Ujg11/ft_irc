/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 14:34:06 by agrimald          #+#    #+#             */
/*   Updated: 2024/10/15 17:24:30 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Command.hpp"
# include "../Server.hpp"

class Channel;

class Join : public Command
{
    public:
        void createNewChannel(Client &user, Server &server, const std::string &channelName, const std::string &password);
        void handleExistingChannel(Client &user, Server &server, Channel *channel, const std::string &channelName);
        bool handleJoinChannel(Client &user, Server &server, const std::string &channelName, const std::string &password);
        void execute(Server &server, Client &c, std::vector<std::string> args);
};
