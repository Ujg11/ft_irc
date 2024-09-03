/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 11:06:59 by ojimenez          #+#    #+#             */
/*   Updated: 2024/09/03 12:22:03 by ojimenez         ###   ########.fr       */
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

class Client
{
	private:
		int fd;
		std::string ip;
	
	public:
		Client();
		~Client();
		
		int getFd() const;
		void setFd(int fd);
		void setIP(std::string ip);
};

