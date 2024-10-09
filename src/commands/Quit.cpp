/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 15:37:31 by ojimenez          #+#    #+#             */
/*   Updated: 2024/10/09 15:45:58 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/commands/Quit.hpp"

void Quit::execute(Server &server, Client &c, std::vector<std::string> args)
{
	std::string reason;
	if (args.empty())
		reason = "Leaving...";
	else
		reason = args.at(0);
	if (reason.at(0) == ':')
		reason = reason.substr(1);
	//Enviar missatge RPL_QUIT al client
	std::string message = ":" + c.getPrefix() + " QUIT :" + reason + "\r\n";
	server.broadcastQuitMessage(c, message);
	server.clearClient(c.getFd());
	std::cout << "Client <" << c.getFd() << "> Disconnected" << std::endl;
}
