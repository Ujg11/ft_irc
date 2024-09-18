/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 16:07:00 by ojimenez          #+#    #+#             */
/*   Updated: 2024/09/15 19:21:21 by ojimenez         ###   ########.fr       */
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
		Client *admin;
		std::vector<Client> clients;
		
	public:
		Channel(std::string name, std::string key, Client *admin);
		~Channel();

		void removeClient(int fd);
		bool addClient(Client c);
		const std::vector<Client> getClients() const;
		std::string getName();

		/* ANTHONY ↓: */

		bool isClient(const Client &client) const;
		bool isOperator(const Client &client) const;
		bool removeClient(const Client &client);
		bool isEmpty() const;

		/* TERMINO LO DE ANTHONY ⬆ */
};

