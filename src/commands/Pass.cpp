/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 16:08:55 by ojimenez          #+#    #+#             */
/*   Updated: 2024/09/18 17:23:37 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/commands/Pass.hpp"

void Pass::execute(Server &server, Client &c, std::vector<std::string> args)
{
	std::string response;
	std::cout << "LLEGA A PASS" << std::endl;
	
	if (!args.empty() && args.at(0) == server.getPassword())
	{
		response = "Password accepted.\r\n";
		send(c.getFd(), response.c_str(), response.length(), 0);
		c.hasPass = true;
	}
}
