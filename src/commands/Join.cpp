/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrimald <agrimald@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 17:46:18 by agrimald          #+#    #+#             */
/*   Updated: 2024/09/20 17:46:20 by agrimald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/commands/Join.hpp"

void Join::createNewChannel(Client &user, Server &server, const std::string &channelName, const std::string &password)
{
    // Crear un nuevo canal
    Channel *newChannel = server.create_channel(channelName, password, user);
    if (!newChannel)
    {
        std::cerr << "Failed to create new channel" << std::endl;
        return ;
    }

    // Agregar al usuario y hacerlo operador(admin)
    newChannel->addAdmin(user);
    newChannel->addClient(user);
    user.addJoinedChannel(channelName);

    // Notificar a los usuarios del canal
    std::string response = ":" + user.getNickname() + " JOIN " + channelName + "\r\n";
    server.message.sendMessage(user, response);

    // Enviar los nombers del canal al usuario
    server.message.sendMessage(user, "Bienvenidos al canal " + channelName + "\r\n");
}

void Join::handleExistingChannel(Client &user, Server &server, Channel *channel, const std::string &channelName, const std::string &password)
{
    std::string response;

    // Verificar modos del canal ('i', 'l', 'k')
    if (channel->isInvitedOnly() && !channel->isInvited(user))
    {
        response = server.message.getMessage(473, user); // No esta invitado
        server.message.sendMessage(user, response);
        return ;
    }

    if (channel->isFull())
    {
        response = server.message.getMessage(471, user); // Canal lleno
        server.message.sendMessage(user, response);
        return ;
    }

    if (!password.empty() && channel->getKey() != password)
    {
        response = server.message.getMessage(475, user); // Contrasena incorrecta
        server.message.sendMessage(user, response);
        return ;
    }

    // Agregar usuario al canal
    channel->addClient(user);
    user.addJoinedChannel(channelName);

    // Notificar a los usuarios del canal
    response = ":" + user.getNickname() + " JOIN " + channelName + "\r\n";
    channel->broadcastMessage(response, user);

    // Enviar el topico del canal si existe
    if (!channel->getTopic().empty())
    {
        response = ":" + server.getServerName() + " 332 " + user.getNickname() + " " + channelName + " :" + channel->getTopic() + "\r\n";
        server.message.sendMessage(user, response);
    }

    // Enviar los nombres del canal
    server.message.sendMessage(user, "Usuario en el canal: \r\n"); // Enviar nombres de usuario
}

bool Join::handleJoinChannel(Client &user, Server &server, const std::string &channelName, const std::string &password)
{
    if (channelName.empty() || channelName[0] != '#')
    {
        server.message.sendMessage(user, "El nombre del canal debe de empezar con #\r\n");
        return false;
    }

    Channel *channel = server.getChannel(channelName);
    if(channel)
    {
        handleExistingChannel(user, server, channel, channelName, password);
        return true;
    }
    else
    {
        createNewChannel(user, server, channelName, password);
        return true;
    }
}

void Join::execute(Server &server, Client &c, std::vector<std::string> args)
{
    std::string response;

    // Verifica que el comando tenga los argumentos correctos
    if (args.size() < 2)
    {
        response = server.message.getMessage(461, c); // "JOIN" requiere mas parametros
        server.message.sendMessage(c, response);
        return ;
    }

    std::string channelsStr = args[1];
    std::string passwordsStr = args.size() > 2 ? args[2] : "";

    StringHandler strTool;

    std::vector<std::string> channels = strTool.stringSplit(channelsStr, ",");
    std::vector<std::string> passwords = strTool.stringSplit(passwordsStr, ",");

    for (size_t i = 0; i < channels.size(); i++) // o ++i
    {
        std::string channelName = channels[i];
        std::string password = i < passwords.size() ? passwords[i] : "";

        if (!handleJoinChannel(c, server, channelName, password))
        {
            response = "ERROR: No such channel: " + channelName + "\r\n";
            server.message.sendMessage(c, response);
        }
    }
}