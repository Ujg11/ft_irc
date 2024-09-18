/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrimald <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 17:52:30 by agrimald          #+#    #+#             */
/*   Updated: 2024/09/17 17:52:31 by agrimald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/commands/Kick.hpp"

/* FindReason: encuentra el motivo del Kick a partir de los argumentos. */
std::string Kick::FindReason(std::vector<std::string> &args)
{
    if (args.size() > 2)
    {
        std::string reason = argv[2];
        if (reason[0] == ":") // Elimina el ';' del motivo
            return substr(1);
        return reason;
    }
    return ""; // no hay motivo proporcionado
}
/* Ejemplo  de esta funcion (FindReason):


    Si "args = {"#channel", "usuario", ":motivo del kick"}" , esta funcion
    devolveria "motivo del kick". Si no hay motivo, devuelve una cadena vacia.
*/

/* SplitChannels: Divide el primer argumento (el nombre del canal) en multiples canales
    si estan separados por comas. */
std::vector<std::string> Kick::SplitChannels(const std::string &channelList)
{
    std::vector<std::string> channels;
    std::stringstream ss(channelList);
    std::string channel;
    while (std::getline(ss, channel, ','))
    {
        if (!channel.empty())
            channels.push_back(channel);
    }
    return channels;
}
/* Ejemplo de esta funcion (SplitChannels):

    Si "argv[0] = "#channel1, #channel2", devolveria un vector con "["#channel1",
    "#channel2"]"
*/


/* Kick: Esta funcion sigue los mismos pasos descritos anteriormente para implementar
    el comando KICK. */
void Kick::execute(Server &server, Client &c, std::vector<std::string> args)
{
    // Verificar si se proporcionaron suficientes parametros
    if (args.size() < 2)
    {
        c.sendError(" 461 ", c.getNickname(), "KICK", "Not enough parameters");
        return ;
    }

    // Obtener el canal o canales del usuario para expulsar
    std::vector<std::string> channels = SplitChannels(argv[0]);
    std::string userToKick = argv[1];
    std::string reason = FindReason(args); // Motivo opcional

    // Iterar sobre el canal
    for (size_t i = 0; i < channels.size(); i++)
    {
        std::string &channelName = channels[i];

        // Verificar si el canal existe
        Channel *channel = server.getChannel(channelName);
        if (!channel)
        {
            c.sendError(" 403 ", c.getNickname(), channelName, "No such channel");
            continue;
        }

        // Verificar si el cliente que ejecuta el KICK esta en el canal
        if (!channel->isClient(c.getFd))
        {
            c.sendError(" 442 ", c.getNickname(), channelName, "You're not on that channel");
            continue;
        }

        // Verificar si el cliente es un operador del canal
        if (!channel->isOperator(c.getFd))
        {
            c.sendError(" 482 ", c.getNickname(), channelName, "You are not channel operator");
            continue;
        }

        // Verificar si el usuario a expulsar esta en el canal
        Client *clientToKick = channel->getClientByName(userToKick);
        if (!clientToKick)
        {
            c.sendError(" 441 ", c.getNickname(), channelName, "They aren't on that channel");
            continue;
        }

        // Realizar el KICK: Notificar a todos en el canal
        std::stringstream ss;
        ss << ":" << c.getNickname() << "!" << c.getUsername() << "@" << c.getIp() 
            << " KICK " << channelName << " " << userToKick;
        if (!reason.empty()) ss << ":" << reason;
        ss << "\r\n";
        channel->broadcast(ss.str());

        // Remover al cliente del canal
        channel->removeClient(clientToKick->getFd());

        // Si el canal queda vacio, eliminar del servidor
        if (channel->isEmpty())
        {
            server.removeChannel(channelName);
        }
    }
}
/* Ejemplo de la funcion (KICK):

    Supongamos que el cliente envia el comando:

    KICK #channel usuario_a_expulsar :motivo del kick

    el vector args sera:

    args = {"#channel", "usuario_a_expulsar", ":motivo del kick"};

    1. SplitChannels("#channel") devolvera ["#channel"].
    2. FindReason(args) devolvera "motivo del kick".
    3. Verificamos:
        . Si el "channel" existe.
        . Si el cliente que ejecuta el comando es operador en ese canal.
        . Si el usuario a expulsar esta en ese canal.
    4. Si todo esta correcto, expulsamos al usuario y notificamos a todos en el canal.
*/

/* NOTA:

    Cosas que no tenemos:

        . Funcion "sendError" -> ubicacion en "Funcion (kICK), linea 62".
        . Funcion "getChannel" -> ubicacion linea 77. ✅
        . Funcion "isClient" -> ubicacion linea 85. ✅
        . Funcion "isOperator" -> ubicacion linea 92. ✅
        . Funcion "getClientByName" -> ubicacion linea 99. ✅
        . Funcion "broadcast" -> ubicacion linea 112 (es una funcion nuestra, se llama sendMessage (Leer NOTA 2)).
        . Funcion "removeClient" -> ubicacion linea 115. ✅
        . Funcion "isEmpty" -> ubicacion linea 118. ✅
        . Funcion "removeChannel" -> ubicacion linea 120. ✅

        Los que tengan check verde, estan creados, solo hace falta comprobar que funcionan
            correctamente.
*/

/* NOTA 2:

    La funcion broadcast es lo que tenemos en la clase Message:

        Ejemplo de broadcast:

            void Channel::broadcast(const std::string &message)
            {
                for (size_t i = 0; i < clients.size(); ++i)
                {
                    // Enviar el mensaje a cada cliente en el canal
                    send(clients[i].getFd(), message.c_str(), message.size(), 0);
                }
            }

    El nuestro se encuentra en Message, pensar una manera de en la linea 112, usar nuestro broadcast
*/