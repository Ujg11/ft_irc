/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrimald <agrimald@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 18:51:39 by agrimald          #+#    #+#             */
/*   Updated: 2024/09/30 18:51:40 by agrimald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include "../Client.hpp"

class Server;

class Command
{
    public:
        virtual ~Command() {}
        virtual void execute(Server &server, Client &c, std::vector<std::string> args) = 0;
};