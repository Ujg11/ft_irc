/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 17:52:16 by agrimald          #+#    #+#             */
/*   Updated: 2024/10/09 19:32:33 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include "Command.hpp"
# include "../Server.hpp"

class Kick: public Command
{
    public:
        std::vector<std::string> SplitUsers(const std::string &userList);
        std::string FindReason(std::vector<std::string> &args);
        std::vector<std::string> SplitChannels(const std::string &channelList);
        void execute(Server &server, Client &c, std::vector<std::string> args);
};
