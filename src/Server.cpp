/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 11:45:05 by ojimenez          #+#    #+#             */
/*   Updated: 2024/09/03 15:19:18 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

bool Server::signal = false;

Server::Server()
{
	this->socketFd = -1;
}

Server::~Server()
{
}

//Iniciem el servidor i esperem per obtenir senyals
//Mirem si algun nou client intenta conectar-se o bé un client registrat envia informació
void Server::serverInit()
{
	this->port = 4242;
	serverSocket();
	std::cout << "Server <" << socketFd << "> Connected" << std::endl;
	std::cout << "Waitig connections..." << std::endl;

	while (Server::signal == false)
	{
		//Cridem la funció poll per a tots els fds registrats
		if ((poll(&fds[0], fds.size(), -1) == -1) && Server::signal == false)
			throw (std::runtime_error("Error! poll() failed"));
		for (size_t i = 0; i < fds.size(); i++)
		{
			//Comprobem si hi ha dades per llegir
			if (fds[i].revents & POLLIN)
			{
				if (fds[i].fd == socketFd)
					acceptNewClient();
				else
					recieveNewData(fds[i].fd);
			}
		}
	}
	closeFds();
}

//Creem el socket del servidor per poder escoltar i aceptar conexions entrants
//sockaddr_in:  conte informacio sobre la direccio i el port del socket
//pollfd:  estructura que utilitza poll() per monitorejar diferents fds
void Server::serverSocket()
{
	struct sockaddr_in add; //representa una IPv4
	struct pollfd newPoll;

	add.sin_family = AF_INET; //Indiquem que s'utilitza IPv4
	add.sin_port = htons(this->port); //El port es converteix en bytes (big rndian) per er compatible amb el protocol de xarxa
	add.sin_addr.s_addr = INADDR_ANY; //El servidor acceptara conexions en totes les IP locals
	
	//Creem un Socket per a conexions TCP/IP. Si falla retorna -1
	//SOCK_STREAM: indica conexio orientada a la transmisio continua de dades (TCP)
	//0: Indica que utilitzarem el protocol per defecte de la combinacio dels altres parametres
	socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketFd == -1)
		throw (std::runtime_error("Error! Failed to crete socket"));
	int en = 1;
	//Configurem el socket perque reutilitzi la direccio. Evitem problema al reiniciar i que digui direccio en us
	if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1)
		throw (std::runtime_error("Error! Failed to set option (SO_REUSEADDR) on socket"));
	//Establim el socket com a no bloquejant (requirement)
	if (fcntl(socketFd, F_SETFL, O_NONBLOCK) == -1)
		throw (std::runtime_error("Error! Failed to bind socket"));
	//Vinculem el socket al port i a la direccio especifica de l'estructura
	if (bind(socketFd, (struct sockaddr *)&add, sizeof(add)) == -1)
		throw(std::runtime_error("Error! Failed to bind socket"));
	//Posem el socket pq estigui escoltant. SOMAXCONN es el num maxim de conexions en cua permeses
	if (listen(socketFd, SOMAXCONN) == -1)
		throw (std::runtime_error("Error! listen() function failed"));

	newPoll.fd = socketFd; //Assignem el fd del socket al camp de pollfd
	newPoll.events = POLLIN; //Fem que poll() monitoreji el socket per veure si hi ha dades entrants
	newPoll.revents = 0; //Aquest camp sera modificat per poll() per indicar events
	fds.push_back(newPoll); //Afegim la estructura al vector de fds que monitoreja poll() 
	
}

void Server::acceptNewClient()
{
	Client				client;
	struct sockaddr_in	cliadd;
	struct pollfd		newPoll;
	socklen_t	len = sizeof(cliadd);

	


}

//Rebem informacio d'un client ja registrat
void Server::recieveNewData(int fd)
{
	
}

void Server::signalHandler(int signum)
{
	(void)signum;
	std::cout << std::endl << "Signal Recieved!" << std::endl;
	Server::signal = true; //Signal true per parar el servidor
}

//Tanquem tots els clients
void Server::closeFds()
{
	for (size_t i = 0; i < clients.size(); i++)
	{
		std::cout << "Client <" << clients[i].getFd() << "> Disconnected" << std::endl;
		close(clients[i].getFd());
	}
	if (socketFd != -1)
	{
		std::cout << "Server <" << socketFd << "> Disconnected" << std::endl;
		close(socketFd);
	}
}

//Borrem un client de la llista e cleints i de la llista d'fds
void Server::clearClient(int fd)
{
	for (size_t i = 0; i < fds.size(); i++)
	{
		if (fds[i].fd == fd)
		{
			fds.erase(fds.begin() + i);
			break ;
		}
	}
	for (size_t i = 0; i < clients.size(); i++)
	{
		if (clients[i].getFd() == fd)
		{
			clients.erase(clients.begin() + i);
			break ;
		}
	}
}

