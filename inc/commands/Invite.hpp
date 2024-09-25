/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 11:15:55 by ojimenez          #+#    #+#             */
/*   Updated: 2024/09/25 11:17:06 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Command.hpp"
# include "../Server.hpp"

class Server;

class Invite: public Command
{
	public:
		void execute(Server &server, Client &c, std::vector<std::string> args);
};

