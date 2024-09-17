/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 11:20:50 by ojimenez          #+#    #+#             */
/*   Updated: 2024/09/10 15:40:23 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"

Client::Client()
{
	
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

std::string Client::getNickname()
{
	return (this->nickname);
}

void Client::setUsername(std::string name)
{
	this->username = name;
}

std::string Client::getUsername()
{
	return (this->username);
}

void Client::setNickname(std::string nik)
{
	this->nickname = nik;
}

std::string Client::getIp() // esto es igual a -> getHost
{
	return (this->ip);
}