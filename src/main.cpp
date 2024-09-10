/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 15:00:51 by ojimenez          #+#    #+#             */
/*   Updated: 2024/09/10 16:17:31 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

int main()
{
    Server server;

    std::cout << "---- SERVER ----" << std::endl;
    try
    {
        signal(SIGINT, Server::signalHandler);
        signal(SIGQUIT, Server::signalHandler);
        server.serverInit(4444, "hola");
    
    }
    catch(const std::exception& e)
    {
        server.closeFds();
        std::cerr << e.what() << std::endl;
    }
    std::cout << "Server Closed!" << std::endl;
    return (0);
}