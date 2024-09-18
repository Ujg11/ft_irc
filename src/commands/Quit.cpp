/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 15:37:31 by ojimenez          #+#    #+#             */
/*   Updated: 2024/09/18 16:32:38 by ojimenez         ###   ########.fr       */
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
    server.clearClient(c.getFd());
    std::cout << "Arriba a QUIT" << std::endl;
}
