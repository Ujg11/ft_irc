/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 11:38:13 by ojimenez          #+#    #+#             */
/*   Updated: 2024/09/24 12:59:00 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Command.hpp"
# include "../Server.hpp"

class Server;

class Privmsg: public Command
{
	public:
		void execute(Server &server, Client &c, std::vector<std::string> args);
};
