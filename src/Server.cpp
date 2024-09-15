/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 11:45:05 by ojimenez          #+#    #+#             */
/*   Updated: 2024/09/15 20:22:22 by ojimenez         ###   ########.fr       */
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
void Server::serverInit(int port, std::string passwd)
{
	this->port = port;
	this->password = passwd;
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
	deleteAllChannels();
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
	struct sockaddr_in	cliAdd;
	struct pollfd		newPoll;
	socklen_t	len = sizeof(cliAdd);
	int			cliFd = accept(socketFd, (sockaddr *)&(cliAdd), &len); //Acceptem el nou client

	if (cliFd == -1)
	{
		std::cout << "Accept function failed" << std::endl;
		return ;
	}
	if (fcntl(cliFd, F_SETFL, O_NONBLOCK) == -1) //Fem que sigui no bloquejant
	{
		std::cout << "fcntl() function in acceptClient() failed" << std::endl;
		return ;
	}
	newPoll.fd = cliFd;
	newPoll.events = POLLIN;
	newPoll.revents = 0;
	fds.push_back(newPoll);

	if (!newClientRequest(client, cliFd))
	{
		clearClient(cliFd);
		close(cliFd);
		return ;
	}
	client.setFd(cliFd);
	client.setIP(inet_ntoa(cliAdd.sin_addr));
	clients.push_back(client); //Afegim el client al vector
	//fds.push_back(newPoll);
	std::cout << "Client whith fd <" << cliFd << "> connected correctly" << std::endl;
}

bool Server::newClientRequest(Client &client, int cliFd)
{
	char buffer[1024];
	std::string pw = "Please, insert the correct password: ";
	std::string nik = "Congratulations, password is correct.\nNikname: ";
	std::string incorrectPw = "Sorry, the password is incorrect :(";
	std::string incorrectNik = "The nickname is in use or empty, please try again: ";
	std::string name = "User name: ";
	std::string incorrectName = "Incorrect or empty name, only characters please.\nUser Name: ";
	std::string success = "Conected correctly! Enjoy :)\n";

	send(cliFd, pw.c_str(), pw.size(), 0);
	std::string pwRecieved = read_message(cliFd);
	if (pwRecieved.empty())
	{
		close(cliFd);
		return (false);
	}
	if (pwRecieved != this->password)
	{
		send(cliFd, incorrectPw.c_str(), incorrectPw.size(), 0);
		close(cliFd);
		return false;
	}
	send(cliFd, nik.c_str(), nik.size(), 0);
	std::string nickname = read_message(cliFd);
	while (!isNicknameValid(nickname))
	{
		send(cliFd, incorrectNik.c_str(), incorrectNik.size(), 0);
		nickname = read_message(cliFd);
		if (nickname.empty())
		{
			close(cliFd);
			return (false);
		}
	}
	client.setNickname(nickname);
	send(cliFd, name.c_str(), name.size(), 0);
	std::string username = read_message(cliFd);
	while (!isNameValid(username))
	{
		send(cliFd, incorrectName.c_str(), incorrectName.size(), 0);
		username = read_message(cliFd);
		if (username.empty())
		{
			close(cliFd);
			return (false);
		}
	}
	client.setUsername(username);
	send(cliFd, success.c_str(), success.size(), 0);
	return (true);
}

std::string Server::read_message(int fd)
{
	std::string message = "";
	char buffer[1024];
	ssize_t bytes;

	memset(buffer, 0, sizeof(buffer));
	while (!strstr(buffer, "\n"))
	{
		memset(buffer, 0, sizeof(buffer));
		bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);
		if (bytes < 0)
		{
			if (errno != EWOULDBLOCK && errno != EAGAIN)
			{
				std::cerr << "Error in recv() function. Client with fd <" << fd << "> disconnected." << std::endl;
				return "";
			}
		}
		else if (bytes == 0)
		{
			std::cout << "Client with fd <" << fd << "> disconnected" << std::endl;
			return "";
		}
		buffer[bytes] = '\0';
		message.append(buffer);
	}
	message.erase(message.size() - 1);
	return (message);
}

bool Server::isNameValid(std::string name)
{
	if (name.empty())
		return (false);
	for (int i = 0; i < name.size(); i++)
	{		
		if (!isspace(name[i]) && (isdigit(name[i]) || !isalpha(name[i])))
			return (false);
	}
	return (true);
}

bool Server::isNicknameValid(std::string n)
{
	if (n.empty())
		return (false);
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->getNickname() == n)
			return (false);
	}
	return (true);
}

//Rebem informacio d'un client ja registrat
void Server::recieveNewData(int fd)
{
	char	buffer[1024];
	ssize_t	bytes;

	memset(buffer, 0, sizeof(buffer));
	bytes = recv(fd, buffer, sizeof(buffer) -1, 0);//La funcio llegeix del fd i ho posa a buffer. El tamany indicat
	if (bytes <= 0)
	{
		std::cout << "Client whith fd <" << fd << "> disconnected" << std::endl;
		clearClient(fd);
		close(fd);
		//Borrar el client dels canals, de la llista de clients i de la llista d'fds
		return ;
	}
	buffer[bytes] = '\0';
	std::cout << "Data from client with fd <" << fd << "> recieved" << std::endl;
	std::string message(buffer);
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->getFd() == fd)
			processMessage(it->getNickname(), message);
	}
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
	//ClearClientFromChannel
}

Channel *Server::create_channel(const std::string &name, const std::string &key, Client *client)
{
	for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if ((*it)->getName() == name)
			return (NULL);
	}
	Channel *c = new Channel(name, key, client);
    if (c == NULL)
    {
        std::cerr << "Error: memory is full" << std::endl;
        return NULL;
    }
	this->channels.push_back(c);
	return (c);
}

void Server::deleteChannel(const std::string &name)
{
	for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        if ((*it)->getName() == name)
        {
            delete *it;
            channels.erase(it);
            std::cout << "Channel " << name << " correctly deleted" << std::endl;
            return;
        }
    }
    std::cerr << "Error: Channel don't found" << name << std::endl;
}

void Server::deleteAllChannels()
{
	for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        delete *it;
    }
    channels.clear();
    std::cout << "All the channels correctly deleted" << std::endl;
}

void Server::processMessage(const std::string &sender, const std::string &bigMessage)
{
	std::istringstream iss(bigMessage);
	std::string order, target, message;

	iss >> order >> target;
	getline(iss, message);
	if (order == "PRIVMSG")
	{
		if (target[0] == '#')
			handleChannelMessage(sender, target, message);
		else
			handlePrivMessag(sender, target, message);
	}

	/*std::istringstream iss(bigMessage);
    std::string command, target;
    iss >> command >> target;

    if (command == "KICK") {
        Kick kickCommand;
        kickCommand.execute(*this, target, findClient(sender), target);
    } else if (command == "JOIN") {
        Join joinCommand;
        joinCommand.execute(*this, target, findClient(sender));
    }*/
}

//Buscamos el destinatario del mensaje y lo enviamos
//Si no lo encontramos mandamos el mensaje de error al emisor
void Server::handlePrivMessag(const std::string &sender, const std::string &reciever, const std::string &message)
{
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->getNickname() == reciever)
		{
			std::string fullMessage = sender + ": " + message + "\n";
			send(it->getFd(), fullMessage.c_str(), fullMessage.size(), 0);
			return ;
		}
	}
	std::string err = "No such user with nickname < " + reciever + " >\n";
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->getNickname() == sender)
		{
			send(it->getFd(), err.c_str(), err.size(), 0);
			return ;
		}
	}
}

void Server::handleChannelMessage(const std::string &sender, const std::string &channel, const std::string &message)
{
	if (!isExistentChannel(channel))
	{
		std::string err = "The channel < " + channel + " > don't exist";
		for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
		{
			if (it->getNickname() == sender)
			{
				send(it->getFd(), err.c_str(), err.size(), 0);
				return ;
			}
		}
	}
	for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if ((*it)->getName() == channel)
		{
			std::vector<Client> channelClients = (*it)->getClients();
			std::string fullMessage = sender + ": " + message + "\n";
			for (std::vector<Client>::iterator i = channelClients.begin(); i != channelClients.end(); ++i)
			{
				if (i->getNickname() != sender)
					send(i->getFd(), fullMessage.c_str(), fullMessage.size(), 0);
			}
			return ;
		}
	}
}

bool Server::isExistentChannel(const std::string &name) 
{
	for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if ((*it)->getName() == name)
			return (true);
	}
	return (false);
}




