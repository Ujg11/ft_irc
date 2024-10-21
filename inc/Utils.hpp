/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrimald <agrimald@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 19:32:12 by agrimald          #+#    #+#             */
/*   Updated: 2024/10/21 19:32:15 by agrimald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include "Server.hpp"
# include "Channel.hpp"
# include "Client.hpp"
# include "Message.hpp"

std::string convertToCRLF(const std::string& input);