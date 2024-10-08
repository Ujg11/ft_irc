/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 17:01:00 by agrimald          #+#    #+#             */
/*   Updated: 2024/09/25 11:34:57 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once

# include <iostream>
# include <cstring>
# include <ctime>
# include "Client.hpp"

struct Message
{
    std::string timeNow() const;
    std::string getMessage(int code, Client &client) const;
    std::string getMessage(int code, Client &client, std::string &cmd) const;
    void sendMessage(const Client &client, std::string message);
    void sendHandShake(Client &c);
};
