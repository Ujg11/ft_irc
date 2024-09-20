/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrimald <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 12:49:41 by agrimald          #+#    #+#             */
/*   Updated: 2024/09/20 12:49:42 by agrimald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/commands/Mode.hpp"

        /* Controla que los usuarios sean OP(operadores - admin)
            MODE tendra los siguientes modos(modificaciones):

            . i: Set/remove Invite-only channel.
            . t: Set/remove the restrictions of the TOPIC command
                 to channel operators.
            . k: Set/remove the cannel key (password).
            . o: Give/take channel operator privilege.
            . l: Set/remove the user limit to channel.

            MODE: tiene acceso a la informacion de los canales y sus permisos
            Ejemplo de un comando en MODE:
            
                MODE #cannal +i -t +pk -pk
        */

void Mode::execute(Server &server, Client &c, std::vector<std::string> args)
{
    
}