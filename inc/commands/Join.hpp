/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrimald <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 14:34:06 by agrimald          #+#    #+#             */
/*   Updated: 2024/09/18 14:34:09 by agrimald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Command.hpp"

class Join : public Command
{
    public:
        ~Join();
        void execute(Server &server, Client &c, std::vector<std::string> args);
};
