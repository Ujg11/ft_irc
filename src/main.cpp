/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 15:00:51 by ojimenez          #+#    #+#             */
/*   Updated: 2024/09/17 12:35:26 by ojimenez         ###   ########.fr       */
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
        server.serverInit(42069, "hola");
    
    }
    catch(const std::exception& e)
    {
        server.closeFds();
        std::cerr << e.what() << std::endl;
    }
    std::cout << "Server Closed!" << std::endl;
    return (0);
}