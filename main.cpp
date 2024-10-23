/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 15:00:51 by ojimenez          #+#    #+#             */
/*   Updated: 2024/10/23 12:06:33 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/Server.hpp"

int main(int argc, char **argv)
{
	Server server;
	char* endptr;
	std::string password = argv[2];

	if (argc != 3)
	{
		std::cerr << "There are not enough arguments, <port> <password>" << std::endl;
		return 1;
	}
    int port = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0' || port < 1024 || port > 65535)
	{
        std::cerr << "Invalid port number" << std::endl;
        return 1;
    }
	std::cout << "---- SERVER ----" << std::endl;
	try
	{
		signal(SIGINT, Server::signalHandler);
		signal(SIGQUIT, Server::signalHandler);
		server.serverInit(port, password);
	
	}
	catch(const std::exception& e)
	{
		server.closeFds();
		std::cerr << e.what() << std::endl;
	}
	std::cout << "Server Closed!" << std::endl;
	return (0);
}