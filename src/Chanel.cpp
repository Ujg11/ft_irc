/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chanel.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 16:08:06 by ojimenez          #+#    #+#             */
/*   Updated: 2024/09/07 16:24:34 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Chanel.hpp"

Chanel::Chanel()
{
    this->name = name;
}

Chanel::Chanel(std::string name)
{
    this->name = name;
}

Chanel::~Chanel()
{   
}

void Chanel::removeClient(int fd)
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

bool Chanel::addClient(Client c)
{
    for (std::vector<Client>::iterator it = clients.begin(); it  != clients.end(); ++it)
    {
        if (it->getFd() == c.getFd())
            return (false);
    }
    clients.push_back(c);
    return (true);
}
