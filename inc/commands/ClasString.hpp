/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClasString.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrimald <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 16:50:01 by agrimald          #+#    #+#             */
/*   Updated: 2024/09/20 16:50:03 by agrimald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include "Command.hpp"
# include "../Server.hpp"

class StringHandler
{
    public:
		std::vector<std::string> stringSplit(std::string str, std::string delimiter);
};
