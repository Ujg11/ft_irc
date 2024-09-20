/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 16:08:06 by ojimenez          #+#    #+#             */
/*   Updated: 2024/09/20 11:53:42 by ojimenez         ###   ########.fr       */
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

void Channel::removeClient(const Client &client)
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

void Channel::broadcast(const std::string &message)
{
    for (size_t i = 0; i < clients.size(); i++) // o ++i
    {
        send(clients[i].getFd(), message.c_str(), message.length(), MSG_DONTWAIT);
    }
}

/* FUNCIONES QUE UTILIZAMOS EN JOIN AQUI ABAJO */

bool Channel::isInvitedOnly() const
{
    return invitedOnly; // Flag
}

bool Channel::isInvited(const Client &user) const
{
    return std::find(invitedClients.begin(), invitedClients.end(), user.getNickname()) != invitedClients.end();
}

bool Channel::isFull() const
{
    return clients.size() >= maxClients; // `maxClients` es el límite máximo de usuarios en el canal
}

std::string Channel::getKey() const
{
    return key; // `channelKey` sería la contraseña del canal, hay una key en private, podemos utilizar eso?
}

void Channel::broadcastMessage(const std::string &message, const Client &c)
{
    for (size_t i = 0; i < clients.size(); i++) // o ++i
    {
        if (clients[i].getNickname() != c.getNickname())
            send(clients[i].getFd(), message.c_str(), message.length(), 0);
    }
}

std::string Channel::getTopic() const
{
    return topic;
}

std::vector<std::string> Channel::stringSplit(std::string str, std::string delimiter)
{
    std::vector<std::string> split;
    std::string substr;
    size_t start, end;

    start = 0;
    while ((end = str.find(delimiter, start)) != std::string::npos)
    {
        substr = str.substr(start, end - start);
        if (!substr.empty())
            split.push_back(substr);
        start = end + delimiter.size();
    }
    substr = str.substr(start);
    if (!substr.empty())
        split.push_back(substr);
    return split;
}

/* Si esto no va aqui hay otra modificacion

    bool Channel::isInvitedOnly() const {
    return !invitedClients.empty();  // Si hay clientes invitados, el canal está en modo "solo por invitación"
}

bool Channel::isInvited(const Client &user) const {
    // Verifica si el nickname del cliente está en la lista de invitados
    return std::find(invitedClients.begin(), invitedClients.end(), user.getNickname()) != invitedClients.end();
}

void Channel::inviteClient(const Client &user) {
    // Agregar al cliente a la lista de invitados si no está ya en la lista
    if (!isInvited(user)) {
        invitedClients.push_back(user.getNickname());
    }
}


void Join::handleExistingChannel(Client &user, Server &server, Channel *channel, const std::string &channelName, const std::string &password)
{
    std::string response;

    // Verificar modos del canal ('i', 'l', 'k')
    if (channel->isInvitedOnly() && !channel->isInvited(user))  // Revisa si el canal es solo por invitación
    {
        response = server.message.getMessage(473, user); // No está invitado
        server.message.sendMessage(user, response);
        return;
    }

    if (channel->isFull())
    {
        response = server.message.getMessage(471, user); // Canal lleno
        server.message.sendMessage(user, response);
        return;
    }

    if (!password.empty() && channel->getKey() != password)
    {
        response = server.message.getMessage(475, user); // Contraseña incorrecta
        server.message.sendMessage(user, response);
        return;
    }

    // Agregar usuario al canal
    channel->addClient(user);
    user.channelsJoined.push_back(channelName);

    // Notificar a los usuarios del canal
    response = ":" + user.getNickname() + " JOIN " + channelName + "\r\n";
    channel->broadcastMessage(response);

    // Enviar el tópico del canal si existe
    if (!channel->getTopic().empty())
    {
        response = ":" + server.getServerName() + " 332 " + user.getNickname() + " " + channelName + " :" + channel->getTopic() + "\r\n";
        server.message.sendMessage(user, response);
    }

    // Enviar los nombres del canal
    server.message.sendMessage(user, "Usuario en el canal: \r\n"); // Enviar nombres de usuario
}


void Invite::execute(Server &server, Client &user, const std::string &channelName, const std::string &nickname)
{
    Channel *channel = server.getChannel(channelName);
    if (!channel) {
        server.message.sendMessage(user, "ERROR: No such channel: " + channelName + "\r\n");
        return;
    }

    Client *invitedUser = server.getClientByNickname(nickname);
    if (!invitedUser) {
        server.message.sendMessage(user, "ERROR: No such user: " + nickname + "\r\n");
        return;
    }

    // Agregar el usuario a la lista de invitados del canal
    channel->inviteClient(*invitedUser);

    // Notificar al usuario invitado
    std::string response = ":" + user.getNickname() + " INVITE " + invitedUser->getNickname() + " :" + channelName + "\r\n";
    server.message.sendMessage(*invitedUser, response);
}

*/