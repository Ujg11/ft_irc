/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrimald <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 19:47:08 by agrimald          #+#    #+#             */
/*   Updated: 2024/09/19 19:47:09 by agrimald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/commands/Command.hpp"

void Command::execute(Server &server, Client &c, std::vector<std::string> args)
{
    if (args.empty())
        return ;
    
    std::string command = argv[0];

    if (command == "JOIN")
    {
        handleJoin(server, c, args);
    }
}

/* NOTA:

    ideas de como podemos hacer el execute con el join

    class Command {
public:
    void execute(Server &server, Client &client, std::vector<std::string> args) {
        if (args.empty()) {
            return;
        }

        std::string command = args[0];

        if (command == "JOIN") {
            handleJoin(server, client, args);
        }
        // Aquí puedes agregar más comandos en el futuro
    }

private:
    void handleJoin(Server &server, Client &client, const std::vector<std::string> &args) {
        std::string response;

        if (args.size() < 2) {
            response = server.message.getMessage(461, client); // "JOIN" requiere más parámetros
            server.message.sendMessage(client, response);
            return;
        }

        std::string channelsStr = args[1];
        std::string passwordsStr = args.size() > 2 ? args[2] : "";

        std::vector<std::string> channels = strTool.stringSplit(channelsStr, ',');
        std::vector<std::string> passwords = strTool.stringSplit(passwordsStr, ',');

        for (size_t i = 0; i < channels.size(); i++) {
            std::string channelName = channels[i];
            std::string password = i < passwords.size() ? passwords[i] : "";

            if (!handleJoinChannel(client, server, channelName, password)) {
                response = "ERROR: No such channel: " + channelName + "\r\n";
                server.message.sendMessage(client, response);
            }
        }
    }

    bool handleJoinChannel(Client &user, Server &server, const std::string &channelName, const std::string &password) {
        if (channelName.empty() || channelName[0] != '#') {
            server.message.sendMessage(user, "El nombre del canal debe empezar con #\r\n");
            return false;
        }

        Channel *channel = server.getChannel(channelName);
        if (channel) {
            handleExistingChannel(user, server, channel, channelName, password);
            return true;
        } else {
            createNewChannel(user, server, channelName, password);
            return true;
        }
    }

    void createNewChannel(Client &user, Server &server, const std::string &channelName, const std::string &password) {
        Channel *newChannel = server.create_channel(channelName, password, user);
        if (!newChannel) {
            std::cerr << "Failed to create new channel" << std::endl;
            return;
        }
        newChannel->addAdmin(user);
        newChannel->addClient(user);
        user.channelsJoined.push_back(channelName);

        std::string response = ":" + user.getNickname() + " JOIN " + channelName + "\r\n";
        server.message.sendMessage(user, response);
        server.message.sendMessage(user, "Bienvenidos al canal " + channelName + "\r\n");
    }

    void handleExistingChannel(Client &user, Server &server, Channel *channel, const std::string &channelName, const std::string &password) {
        std::string response;

        if (channel->isInvitedOnly() && !channel->isInvited(user)) {
            response = server.message.getMessage(473, user); // No está invitado
            server.message.sendMessage(user, response);
            return;
        }

        if (channel->isFull()) {
            response = server.message.getMessage(471, user); // Canal lleno
            server.message.sendMessage(user, response);
            return;
        }

        if (!password.empty() && channel->getKey() != password) {
            response = server.message.getMessage(475, user); // Contraseña incorrecta
            server.message.sendMessage(user, response);
            return;
        }

        channel->addClient(user);
        user.channelsJoined.push_back(channelName);

        response = ":" + user.getNickname() + " JOIN " + channelName + "\r\n";
        channel->broadcastMessage(response);

        if (!channel->getTopic().empty()) {
            response = ":" + server.getServerName() + " 332 " + user.getNickname() + " " + channelName + " :" + channel->getTopic() + "\r\n";
            server.message.sendMessage(user, response);
        }

        server.message.sendMessage(user, "Usuarios en el canal: \r\n"); // Enviar nombres de usuario
    }
};

// Método para procesar el comando en tu servidor
void processCommand(Server &server, Client &client, const std::string &commandLine) {
    std::istringstream iss(commandLine);
    std::string command;
    std::vector<std::string> args;

    iss >> command; // Leer el comando
    std::string arg;
    while (iss >> arg) {
        args.push_back(arg); // Leer argumentos
    }

    Command cmd;
    cmd.execute(server, client, args);
}


*/