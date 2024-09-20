/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 15:00:51 by ojimenez          #+#    #+#             */
/*   Updated: 2024/09/20 13:01:59 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

/*int main()
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
}*/

// Corrigiendo main

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "There are not enough arguments, <password> <port>" << std::endl;
        return 1;
    }
    std::string port = argv[1];
    std::string password = argv[2];

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