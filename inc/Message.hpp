/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrimald <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 17:01:00 by agrimald          #+#    #+#             */
/*   Updated: 2024/09/16 17:01:02 by agrimald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once

# include <iostream>
# include <cstring>
# include <ctime>
# include "Client.hpp"
# include "Server.hpp"

struct Message
{
    std::string timeNow() const;
    std::string getMessage(int code, Client &client) const;
    void sendMessage(const Client &client, std::string message);
};
