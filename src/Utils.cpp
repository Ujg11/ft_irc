/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrimald <agrimald@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 19:31:51 by agrimald          #+#    #+#             */
/*   Updated: 2024/10/21 19:31:52 by agrimald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../inc/Channel.hpp"
# include "../inc/Client.hpp"
# include "../inc/Message.hpp"
# include "../inc/Server.hpp"
# include "../inc/Utils.hpp"
# include "Utils.hpp"

std::string convertToCRLF(const std::string& input)
{
    std::string result;

    result.reserve(input.size() + 2);
    for (size_t i = 0; i < input.size(); i++)
    {
        if (input[i] == '\n')
        {
            if (i == 0 || input[i - 1] != '\r')
            {
                result += '\r';
            }
        }
        result += input[i];
    }
    if (result.empty() || result[result.size() - 1] != '\n' || (result.size() > 1 && result[result.size() - 2] != '\r' ))
    {
        result += "\r\n";
    }
    return result;
}