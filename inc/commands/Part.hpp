/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 14:21:24 by ojimenez          #+#    #+#             */
/*   Updated: 2024/09/25 14:25:13 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Command.hpp"
# include "../Server.hpp"

class Server;

class Part: public Command
{
	public:
		void execute(Server &server, Client &c, std::vector<std::string> args);
};

