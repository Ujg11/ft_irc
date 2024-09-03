/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 11:20:50 by ojimenez          #+#    #+#             */
/*   Updated: 2024/09/03 15:08:03 by ojimenez         ###   ########.fr       */
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
