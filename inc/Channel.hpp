/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 16:07:00 by ojimenez          #+#    #+#             */
/*   Updated: 2024/09/10 16:05:36 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Client.hpp"
# include "Server.hpp"

class Channel
{
	private:
		std::string name;
		std::vector<Client> clients;
		
	public:
		Channel(std::string name);
		~Channel();

		void removeClient(int fd);
		bool addClient(Client c);
		const std::vector<Client> getClients() const;
		std::string getName();
};

