/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Who.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 18:12:34 by ojimenez          #+#    #+#             */
/*   Updated: 2024/10/15 18:14:35 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Command.hpp"
# include "../Server.hpp"

class Channel;

class Who: public Command
{
	public:
		void execute(Server &server, Client &c, std::vector<std::string> args);
};