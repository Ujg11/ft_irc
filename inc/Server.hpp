/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 11:44:15 by ojimenez          #+#    #+#             */
/*   Updated: 2024/09/03 15:07:50 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include <vector>
# include <sys/socket.h> //socket()
# include <sys/types.h> //socket()
# include <netinet/in.h> //sockaddr_in
# include <fcntl.h> //fcntl()
# include <unistd.h> //close()
# include <arpa/inet.h> //inet_ntoa()
# include <poll.h> //poll()
# include <csignal> //signal()
# include <cstring>

# include "../inc/Client.hpp"

class Server
{
	private:
		int			port; //port del servidor
		int			socketFd;
		static bool	signal;
		std::vector<Client> clients;
		std::vector<struct pollfd> fds; //Vector de pollfd
		std::string	password;
	
	public:
		Server();
		~Server();
		
		void serverInit(int port, std::string passwd);
		void serverSocket();
		void acceptNewClient();
		void recieveNewData(int fd);

		static void signalHandler(int signum);
		
		void closeFds();
		void clearClient(int fd);
		
};

