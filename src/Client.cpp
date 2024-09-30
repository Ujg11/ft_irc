/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 11:20:50 by ojimenez          #+#    #+#             */
/*   Updated: 2024/09/20 14:24:02 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"
# include <sstream>
# include <vector>
# include <string>
# include <algorithm>

Client::Client()
{
	this->hasPass = false;
	this->hasNick = false;
	this->hasName = false;
	this->handShake = false;
}

Client::~Client()
{
	
}

int Client::getFd() const
{
	return (this->fd);
}

void Client::setFd(int fd)
{
	this->fd = fd;
}

void Client::setIP(std::string ip)
{
	this->ip = ip;
}

std::string Client::getNickname() const
{
	return (this->nickname);
}

void Client::setUsername(std::string name)
{
	this->username = name;
}

std::string Client::getUsername() const
{
	return (this->username);
}

void Client::setNickname(std::string nik)
{
	this->nickname = nik;
}

std::string Client::getIp() const // esto es igual a -> getHost
{
	return (this->ip);
}

std::string Client::getPrefix() const
{
	return (getNickname() + "!" + getUsername() + "@" + getIp());
}

std::vector<std::string> Client::getJoinedChannels() const
{
	return (this->joinedChannels);
}

void Client::addJoinedChannel(const std::string &channelName)
{
	if (std::find(joinedChannels.begin(), joinedChannels.end(), channelName) == joinedChannels.end())
		joinedChannels.push_back(channelName);
}
	
void Client::removeJoinedChannel(const std::string &channelName)
{
	std::vector<std::string>::iterator it = std::find(joinedChannels.begin(), joinedChannels.end(), channelName);
	if (it != joinedChannels.end())
		joinedChannels.erase(it);
}

void Client::sendError(int errorCode, const std::string &nickname, const std::string &command, const std::string &message)
{
	std::stringstream ss;
	ss << ":" << nickname << " " << errorCode << " " << command << " " << message << "\r\n";
	std::string fullMessage = ss.str();

	send(this->getFd(), fullMessage.c_str(), fullMessage.length(), MSG_DONTWAIT);
}

bool Client::channelsJoined(const std::string &channelName)
{
	return std::find(joinedChannels.begin(), joinedChannels.end(), channelName) != joinedChannels.end();
}

bool Client::operator==(const Client &other) const
{
	return this->getFd() == other.getFd(); // Comparamos por el descriptor de archivo (fd), o podrías usar otro atributo como el nickname
}