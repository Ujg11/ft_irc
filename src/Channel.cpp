/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 16:08:06 by ojimenez          #+#    #+#             */
/*   Updated: 2024/09/18 11:04:49 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"

Channel::Channel(std::string name, std::string key, Client &admin)
{
    this->name = "#" + name;
    this->key = key;
    addAdmin(admin);
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

/* ANTHONY ↓: */

void Channel::addAdmin(const Client &c)
{
    admins.push_back(c);
}

bool Channel::isClient(const Client &client) const
{
    for (size_t i = 0; i < clients.size(); i++) // o poner ++i
    {
        if (clients[i].getFd() == client.getFd())
            return true; // El cliente esta en el canal.
    }
    return false; // El cliente no esta en el canal.
}

bool Channel::isOperator(const Client &client) const
{
    for (size_t i = 0; i < admins.size(); i++) // o poner ++i
    {
        if (admins[i].getFd() == client.getFd())
            return true; // El cliente es un operador.
    }
    return false; // El cliente no es un operador.
}

bool Channel::removeClient(const Client &client)
{
    for (size_t i = 0; i < clients.size(); i++) // o poner ++i
    {
        if (clients[i].getFd() == client.getFd())
        {
            clients.erase(clients.begin() + i); // Elimina el cliente del canal.
            break;
        }
    }
}

bool Channel::isEmpty() const
{
    return clients.empty(); // Retorna true si no hay clientes en el canal.
}

/* NOTA:

    . En estos 4 metodos que hice no hay variable operador y tampoco clients
        tomar en cuenta eso y solucionar mas adelante.
    . Tampoco hay getName, crearlo mas adelante.

*/

/* TERMINO LO DE ANTHONY ⬆ */