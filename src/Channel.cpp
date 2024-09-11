/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 16:08:06 by ojimenez          #+#    #+#             */
/*   Updated: 2024/09/11 12:54:08 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"

Channel::Channel(std::string name)
{
    this->name = "#" + name;
}

Channel::~Channel()
{   
}

void Channel::removeClient(int fd)
{
    for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
    {
        if (it->getFd() == fd)
        {
            clients.erase(it);
            break ;
        }
    }
}

bool Channel::addClient(Client c)
{
    for (std::vector<Client>::iterator it = clients.begin(); it  != clients.end(); ++it)
    {
        if (it->getFd() == c.getFd())
            return (false);
    }
    clients.push_back(c);
    return (true);
}

const std::vector<Client> Channel::getClients() const
{
    return (clients);
}

std::string Channel::getName()
{
    return (name);
}
