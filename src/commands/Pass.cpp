/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 16:08:55 by ojimenez          #+#    #+#             */
/*   Updated: 2024/10/09 17:31:31 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/commands/Pass.hpp"

void Pass::execute(Server &server, Client &c, std::vector<std::string> args)
{
	std::string response;
	if (c.hasPass)
	{
		response = "You have already authenticated.\r\n";
		send(c.getFd(), response.c_str(), response.length(), 0);
		return;
	}
	if (args.empty())
	{
		response = "No password provided.\r\n";
		send(c.getFd(), response.c_str(), response.length(), 0);
		return;
	}
	if (args.at(0) == server.getPassword())
	{
		response = "Password accepted.\r\n";
		send(c.getFd(), response.c_str(), response.length(), 0);
		c.hasPass = true;
	}
	else
	{
		response = "Incorrect Password.\r\n";
		send(c.getFd(), response.c_str(), response.length(), 0);
	}
}
