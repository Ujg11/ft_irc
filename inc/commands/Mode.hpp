/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrimald <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 12:50:03 by agrimald          #+#    #+#             */
/*   Updated: 2024/09/20 12:50:04 by agrimald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include "Command.hpp"
# include "../Server.hpp"

class Mode : public Command
{
    public:
        void execute(Server &server, Client &c, std::vector<std::string> args);
        bool validModeRequest(Server &server, Client &c, std::vector<std::string> args, Channel *&channel);
        void applyModeChange(Server &server, Client &c, Channel *channel, std::vector<std::string> args);
        void handleInvitedOnly(Channel *channel, bool addMode, Server &server, Client &c);
        void handleTopicMode(Channel *channel, bool addMode, Server &server, Client &c);
        void handleKeyMode(Channel *channel, bool addMode, Server &server, Client &c, std::vector<std::string> &args, size_t &index);
        void handleLimitMode(Channel*channel, bool addMode, Server &server, Client &c, std::vector<std::string> &args, size_t &index);
        void handleOperatorMode(Channel *channel, bool addMode, Server &server, Client &c, std::vector<std::string> &args, size_t &index);
        void broadcastModeChange(Server &server, Client &c, const std::string &channelName, const std::string &modeChange);
};

/*
    NOTAS:

        . Funciones que me hacen falta:
            - Linea 44 funcion getCurrentChannelMode().
            - Linea 121 funcion setInvitedOnly().
            - Linea 128 setTopicRestriccion().
            - Linea 146 funcion setKey().
            - Linea 162 funcion removeKey().
            - Linea 184 funcion setMaxClients().
            - Linea 194 funcion removeUserLimit().
            - Linea 214 funcion addAdmin().
            - Linea 219 funcion removeAdmin().
*/