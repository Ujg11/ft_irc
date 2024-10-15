/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 16:07:00 by ojimenez          #+#    #+#             */
/*   Updated: 2024/10/15 16:16:47 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Client.hpp"
# include "Server.hpp"

class Channel
{
	private:
		std::string name;
		std::string key;
		std::vector<Client> admins;
		std::vector<Client> clients;

		std::vector<std::string> invitedClients;  // Lista de clientes invitados
		bool invitedOnly; // Flag para indicar si el canal es solo para invitados
		size_t maxClients; // Límite máximo de usuarios en el canal
		std::string topic; // Tema del canal
		bool topicRestricted; // Indica si solo los operadores pueden cambiar el topico
		
	public:
		Channel(std::string name, std::string key, Client &admin);
		~Channel();

		void removeClient(int fd);
		void addClient(Client c);
		const std::vector<Client> getClients() const;
		std::string getName();
		void addAdmin(const Client &c);
		void addInvitedClient(const Client &c);
		void removeInvitedClient(const std::string &nick);
		void removeOperator(const Client &c);
		void setTopic(std::string topic);
		bool isInvited(const Client &user) const;
		bool isInvited(const std::string &userName) const;
		std::string getUserList();
		
		/* ANTHONY ↓: */

		bool isClient(const Client &client) const;
		bool isOperator(const Client &client) const;
		void removeClient(const Client &client);
		bool isEmpty() const;

		void broadcast(const std::string &message);

		bool isInvitedOnly() const;
		bool isFull() const;
		std::string getKey() const;
		void broadcastMessage(const std::string &message, const Client &c);
		std::string getTopic() const;

		Client* getClientByName(const std::string &nickname);
		/* TERMINO LO DE ANTHONY ⬆ */

		std::string getCurrentChannelMode();
		void setInvitedOnly(bool isInvitedOnly);
		void setTopicRestriction(bool restricted);
		void setKey(const std::string &channelkey);
		void removeKey();
		void setMaxClients(size_t maxUsers);
		void removeUserLimit();
		//void addAdmin(const Client &client);
		void removeAdmin(const Client &client);
};

