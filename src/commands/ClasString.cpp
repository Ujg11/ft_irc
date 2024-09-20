/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClasString.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrimald <agrimald@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 17:46:13 by agrimald          #+#    #+#             */
/*   Updated: 2024/09/20 17:46:14 by agrimald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/commands/ClasString.hpp"

std::vector<std::string> StringHandler::stringSplit(std::string str, std::string delimiter)
{
    std::vector<std::string> split;
    std::string substr;
    size_t start, end;

    start = 0;
    while ((end = str.find(delimiter, start)) != std::string::npos)
    {
        substr = str.substr(start, end - start);
        if (!substr.empty())
            split.push_back(substr);
        start = end + delimiter.size();
    }
    substr = str.substr(start);
    if (!substr.empty())
        split.push_back(substr);
    return split;
}