/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 11:06:59 by ojimenez          #+#    #+#             */
/*   Updated: 2024/09/18 16:53:41 by ojimenez         ###   ########.fr       */
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
		std::string nickname;
		std::string username;

	public:
		Client();
		~Client();
		
		std::string clientBuffer;
		bool hasPass;
		bool hasNick;
		bool hasName;
		bool hasLogin;
		
		void setFd(int fd);
		void setIP(std::string ip);
		void setNickname(std::string nik);
		void setUsername(std::string name);
		
		int getFd() const;
		std::string getNickname() const;
		std::string getUsername() const;
		std::string getIp() const;
		std::string getPrefix() const;

		/* Anthony */
		void sendError(int errorCode, const std::string &nickname, const std::string &command, const std::string &message);
		bool channelsJoined(const std::string &channelName);
};
