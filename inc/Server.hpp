/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 11:44:15 by ojimenez          #+#    #+#             */
/*   Updated: 2024/10/15 18:29:35 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include <vector>
# include <map>
# include <string>
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

# include "../inc/Message.hpp"
# include "../inc/Client.hpp"
# include "../inc/Channel.hpp"
# include "../inc/commands/Command.hpp"
# include "../inc/commands/Quit.hpp"
# include "../inc/commands/Pass.hpp"
# include "../inc/commands/Kick.hpp"
# include "../inc/commands/Join.hpp"
# include "../inc/commands/Mode.hpp"
# include "../inc/commands/ClasString.hpp"
# include "../inc/commands/Nick.hpp"
# include "../inc/commands/Privmsg.hpp"
# include "../inc/commands/Topic.hpp"
# include "../inc/commands/Invite.hpp"
# include "../inc/commands/Part.hpp"
# include "../inc/commands/Who.hpp"

class Channel;
class Command;

class Server
{
	private:
		int			port; //port del servidor
		int			socketFd;
		static bool	signal;
		std::string serverName;
		std::string	password;
		
		std::vector<Client> clients;
		std::vector<struct pollfd> fds; //Vector de pollfd
		std::vector<Channel *> channels;
		std::map<std::string, Command *> commands;
	
	public:
		Server();
		~Server();
		std::string getPassword();
		std::string getServerName();
		const std::vector<Client> getClients() const;
		Message message;
		
		void serverInit(int port, std::string passwd);
		void serverSocket();
		void acceptNewClient();
		//void recieveNewData(pollfd &polls);
		//bool newClientRequest(Client &client, int cliFd);
		static void signalHandler(int signum);
		void closeFds();
		void clearClient(int fd);
		bool isNicknameValid(std::string n);
		bool isNameValid(std::string name);
		void read_message(pollfd &polls);
		void processMessage(Client &cliente);
		void handlePrivMessag(Client &sender, const std::string &reciever, const std::string &message);
		void handleChannelMessage(Client &sender, const std::string &channel, const std::string &message);
		bool isExistentChannel(const std::string &name);

		bool isClientInChannel(int cliFd, Channel &c);
		void broadcastQuitMessage(Client &client, std::string &message);
		
		//Channel findChannel(const std::string &name);
		Client *findClient(std::string nick);
		Client *findClient(int fd);

		/* ANTHONY ↓: */

		Channel* getChannel(const std::string &channelName);
		//Client* getClientByName(const std::string &nickname);
		void removeChannel(const std::string &channelName);
		void removeClientFromChannel(Client &client, Channel &channel);

		std::string getServerName() const;

		/* TERMINO LO DE ANTHONY ⬆ */
		
		Channel *create_channel(const std::string &name, const std::string &key, Client &client);
		void deleteChannel(const std::string &name);
		void deleteAllChannels();
};

std::vector<std::string> ft_split(const std::string &input, const std::string &delimiter);
