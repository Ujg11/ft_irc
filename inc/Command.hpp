/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 15:13:01 by ojimenez          #+#    #+#             */
/*   Updated: 2024/09/17 17:13:19 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Client.hpp"
# include "Server.hpp"

class Server;

class Command
{
    public:
        virtual ~Command() {}
        virtual void execute(Server &server, Client &c, std::vector<std::string> args) = 0;
};
