/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 11:44:15 by ojimenez          #+#    #+#             */
/*   Updated: 2024/09/17 17:18:12 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include <vector>
# include <map>
# include <sys/socket.h> //socket()
# include <sys/types.h> //socket()
# include <netinet/in.h> //sockaddr_in
# include <fcntl.h> //fcntl()
# include <unistd.h> //close()
# include <arpa/inet.h> //inet_ntoa()
# include <poll.h> //poll()
# include <csignal> //signal()
# include <cstring>
# include <sstream>

# include "../inc/Client.hpp"
# include "../inc/Channel.hpp"
# include "../inc/Command.hpp"
# include "../inc/Quit.hpp"
# include "../inc/Kick.hpp"

class Channel;
class Command;

class Server
{
	private:
		int			port; //port del servidor
		int			socketFd;
		static bool	signal;
		std::vector<Client> clients;
		std::vector<struct pollfd> fds; //Vector de pollfd
		std::string	password;
		std::vector<Channel *> channels;
		std::map<std::string, Command *> commands;
	
	public:
		Server();
		~Server();
		
		void serverInit(int port, std::string passwd);
		void serverSocket();
		void acceptNewClient();
		void recieveNewData(int fd);
		bool newClientRequest(Client &client, int cliFd);
		static void signalHandler(int signum);
		void closeFds();
		void clearClient(int fd);
		bool isNicknameValid(std::string n);
		bool isNameValid(std::string name);
		std::string read_message(int fd);
		void processMessage(const std::string &sender, const std::string &bigMessage);
		void handlePrivMessag(const std::string &sender, const std::string &reciever, const std::string &message);
		void handleChannelMessage(const std::string &sender, const std::string &channel, const std::string &message);
		bool isExistentChannel(const std::string &name);
		//Channel findChannel(const std::string &name);
		Client *findClient(std::string nick);

		/* ANTHONY ↓: */

		Channel* getChannel(const std::string &channelName);
		Client* getClientByName(const std::string &nickname);
		void removeChannel(const std::string &channelName);

		/* TERMINO LO DE ANTHONY ⬆ */
		
		Channel *create_channel(const std::string &name, const std::string &key, Client *client);
		void deleteChannel(const std::string &name);
		void deleteAllChannels();
};

