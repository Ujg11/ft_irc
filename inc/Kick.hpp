/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrimald <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 17:52:16 by agrimald          #+#    #+#             */
/*   Updated: 2024/09/17 17:52:17 by agrimald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include "Command.hpp"

class Kick: public Command
{
    public:
        std::string FindReason(std::vector<std::string> &args);
        std::vector<std::string> SplitChannels(const std::string &channelList);
        void execute(Server &server, Client &c, std::vector<std::string> args);
};