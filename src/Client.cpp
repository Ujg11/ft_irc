/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 11:20:50 by ojimenez          #+#    #+#             */
/*   Updated: 2024/09/18 16:54:31 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"

Client::Client()
{
	this->hasPass = false;
	this->hasNick = false;
	this->hasNick = false;
	this->hasLogin = false;
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