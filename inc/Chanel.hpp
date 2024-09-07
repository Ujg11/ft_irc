/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chanel.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 16:07:00 by ojimenez          #+#    #+#             */
/*   Updated: 2024/09/07 16:23:57 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Client.hpp"
# include "Server.hpp"

class Chanel
{
	private:
		std::string name;
		std::vector<Client> clients;
		
	public:
		Chanel();
		Chanel(std::string name);
		~Chanel();

		void removeClient(int fd);
		bool addClient(Client c);
};

Chanel::Chanel()
{
}

Chanel::~Chanel()
{   
}
