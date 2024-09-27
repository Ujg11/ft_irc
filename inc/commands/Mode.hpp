/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrimald <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 12:50:03 by agrimald          #+#    #+#             */
/*   Updated: 2024/09/20 12:50:04 by agrimald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include "Command.hpp"
# include "../Server.hpp"

class Mode : public Command
{
    public:
        void execute(Server &server, Client &c, std::vector<std::string> args);
};