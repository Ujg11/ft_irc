/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 16:08:06 by ojimenez          #+#    #+#             */
/*   Updated: 2024/09/25 15:06:22 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"

Channel::Channel(std::string name, std::string key, Client &admin)
{
	this->name = name;
	this->key = key;
	this->topic = "";
	this->invitedOnly = false;
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

void Channel::addClient(Client c)
{
	if (!this->isClient(c))
		clients.push_back(c);
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
	if (this->isOperator(c))
		admins.push_back(c);
}

void Channel::addInvitedClient(const Client &c)
{
	for (size_t i = 0; i < invitedClients.size(); i++)
	{
		if (invitedClients[i] == c.getNickname())
			return ;
	}
	invitedClients.push_back(c.getNickname());
}

void Channel::removeInvitedClient(const std::string &nick)
{
	for (std::vector<std::string>::iterator it = invitedClients.begin(); it != invitedClients.end(); ++it)
	{
		if (it->compare(nick) == 0)
		{
			invitedClients.erase(it);
			return ;
		}
	}
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
			return ;
		}
	}
}

void Channel::removeOperator(const Client &c)
{
	for (size_t i = 0; i < admins.size(); i++)
	{
		if (admins[i].getFd() == c.getFd())
		{
			admins.erase(admins.begin() + i);
			return ;
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

//Envia el mensaje a todos los clientes incluido el emisor
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

//Envia el mensaje a todos los clientes menos al emisor
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

void Channel::setTopic(std::string topic)
{
	this->topic = topic;
}

Client* Channel::getClientByName(const std::string &nickname)
{
	for (size_t i = 0; i < clients.size(); i++) // o poner ++i
	{
		if (clients[i].getNickname() == nickname)
			return &clients[i];
	}
	return NULL;
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


/* Anthony anadiendo denuevo */

std::string Channel::getCurrentChannelMode()
{
	std::string modes;

	if (invitedOnly)
		modes += "i";	// Modo de solo invitacion.
	if (!key.empty())
		modes += "k";	// Modo con clave (password).
	if (maxClients > 0)
		modes += "l";	// Modo con limite de usuarios.
	// Puedes agregar mas modos si tienes otras restricciones, como 't' para restringir la edicion de topicos.
	return (modes);
}

void Channel::setInvitedOnly(bool isInvitedOnly)
{
	invitedOnly = isInvitedOnly;
}

void Channel::setTopicRestriction(bool restricted)
{
	topicRestricted = restricted;
}

void Channel::setKey(const std::string &channelkey)
{
	key = channelkey;
}

void Channel::removeKey()
{
	key.clear();
}

void Channel::setMaxClients(size_t maxUsers)
{
	maxClients = maxUsers;
}

void Channel::removeUserLimit()
{
	maxClients = 0;
}

/*void Channel::addAdmin(const Client &client)
{
	admins.push_back(client); // Agregamos al cliente a la lista de admins
}*/

void Channel::removeAdmin(const Client &client)
{
	admins.erase(std::remove(admins.begin(), admins.end(), client), admins.end());
}