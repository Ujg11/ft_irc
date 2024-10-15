/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 11:45:05 by ojimenez          #+#    #+#             */
/*   Updated: 2024/10/15 18:54:36 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

bool Server::signal = false;

Server::Server()
{
	this->socketFd = -1;
	this->serverName = "LosVSCode";
	commands["QUIT"] = new Quit();
	commands["PASS"] = new Pass();
	commands["NICK"] = new Nick();
	commands["MODE"] = new Mode();
	commands["KICK"] = new Kick();
	commands["JOIN"] = new Join();
	commands["PRIVMSG"] = new Privmsg();
	commands["TOPIC"] = new Topic();
	commands["INVITE"] = new Invite();
	commands["PART"] = new Part();
	commands["WHO"] = new Who();
}

Server::~Server()
{
	for (std::map<std::string, Command*>::iterator it = commands.begin(); it != commands.end(); ++it)
	{
		delete it->second;
	}
}

std::string Server::getPassword()
{
	return (this->password);
}

std::string Server::getServerName()
{
	return (this->serverName);
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
					read_message(fds[i]);
			}
			if (fds[i].revents & POLLOUT)
			{
				processMessage(*findClient(fds[i].fd));
				fds[i].revents = POLLIN;
			}
			if (fds[i].revents & (POLLERR | POLLHUP))
			{
				close(fds[i].fd);
				clearClient(fds[i].fd);
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
		close(cliFd);
		return ;
	}
	newPoll.fd = cliFd;
	newPoll.events = POLLIN;
	newPoll.revents = 0;
	fds.push_back(newPoll);
	client.setFd(cliFd);
	client.setIP(inet_ntoa(cliAdd.sin_addr));
	clients.push_back(client);
	std::cout << "Client whith fd <" << cliFd << "> connected correctly" << std::endl;
}

std::vector<std::string> ft_split(const std::string &input, const std::string &delimiter)
{
	std::vector<std::string> result;
	size_t start = 0;
	size_t end = input.find(delimiter);

	while (end != std::string::npos)
	{
		result.push_back(input.substr(start, end - start));
		start = end + delimiter.length();
		end = input.find(delimiter, start);
	}
	if (start < input.length())
		result.push_back(input.substr(start));
	return (result);
}

void Server::processMessage(Client &cliente)
{
	std::vector<std::string> commandsVec = ft_split(cliente.clientBuffer, "\r\n");
	
	for(size_t i = 0; i < commandsVec.size(); i++)
	{
		std::istringstream iss(commandsVec[i]);
		std::string order;
		std::vector<std::string> args;

		iss >> order;
		std::string arg;
		while (iss >> arg)
			args.push_back(arg);
		if (order == "CAP")
		{
			continue;
		}
		else if (order != "PASS" && !cliente.hasPass)
		{
			std::string message = "Please, first introduce the password\r\n";
			send(cliente.getFd(), message.c_str(), message.length(), 0);
			cliente.clientBuffer.clear();
			return ;
		}
		else if (order == "USER")
		{
			if (!args.empty() && !args.at(0).empty())
			{
				cliente.setUsername(args.at(0));
				cliente.hasName = true;
			}
		}
		else if (commands.find(order) != commands.end())
		{
			if (order == "PASS" && !cliente.hasPass)
				commands[order]->execute(*this, cliente, args);
			else if (order != "NICK" && !cliente.handShake)
			{
				std::string message = "Please, NICK and USER needed\r\n";
				send(cliente.getFd(), message.c_str(), message.length(), 0);
				cliente.clientBuffer.clear();
				return ;
			}
			else
				commands[order]->execute(*this, cliente, args);
		}	
		else
		{
			message.sendMessage(cliente, message.getMessage(421, cliente));
		}
	}
	cliente.clientBuffer.clear();
	if (cliente.handShake == false)
	{
		if (cliente.hasPass && cliente.hasNick && cliente.hasName)
		{
			message.sendHandShake(cliente);
			cliente.handShake = true;
			std::cout << "HandShake done in client <" << cliente.getFd() << ">" << std::endl; 
		}	
	}
}

void Server::read_message(pollfd &polls)
{
	
	char buffer[4096];
	ssize_t bytes;
	int fd = polls.fd;
	
	Client *c = findClient(polls.fd);
	if (c == NULL)
	{
		polls.revents = POLLERR;
		return ;
	}
	bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);
	if (bytes <= 0)
	{
		polls.revents = POLLERR;
		std::cout << "Client with fd <" << fd << "> disconnected" << std::endl;
		return ;
	}
	buffer[bytes] = '\0';
	c->clientBuffer.append(buffer);
	if (c->clientBuffer.find("\r\n") != std::string::npos)
	{
		polls.revents = POLLOUT;
		c->clientBuffer.erase(c->clientBuffer.size() - 1);
	}
}

bool Server::isNameValid(std::string name)
{
	if (name.empty())
		return (false);
	for (size_t i = 0; i < name.length(); i++)
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

bool Server::isClientInChannel(int cliFd, Channel &c)
{
	std::vector<Client> llista = c.getClients();
	for (size_t i = 0; i < llista.size(); i++)
	{
		if (llista[i].getFd() == cliFd)
			return (true);
	}
	return (false);
}

void Server::removeClientFromChannel(Client &client, Channel &channel)
{
		channel.removeClient(client);
		channel.removeOperator(client);
		if (channel.isEmpty())
			removeChannel(channel.getName());
}

//Enviamos notificacion a cada canal y eliminamos al cliente de cada canal al que este unido
void Server::broadcastQuitMessage(Client &client, std::string &message)
{
	std::vector<std::string> clientChannels = client.getJoinedChannels();

	for (std::vector<std::string>::iterator it = clientChannels.begin(); it != clientChannels.end(); ++it)
	{
		std::string channelName = *it;
		Channel *chan = getChannel(channelName);
		chan->broadcastMessage(message, client);
		removeClientFromChannel(client, *chan);
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
	close(fd);
}

Channel *Server::create_channel(const std::string &name, const std::string &key, Client &client)
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


//Buscamos el destinatario del mensaje y lo enviamos
//Si no lo encontramos mandamos el mensaje de error al emisor
void Server::handlePrivMessag(Client &sender, const std::string &reciever, const std::string &message)
{
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->getNickname() == reciever)
		{
			std::string fullMessage = ":" + sender.getPrefix() + " PRIVMSG " + reciever + " :" + message;
			send(it->getFd(), fullMessage.c_str(), fullMessage.length(), 0);
			return ;
		}
	}
	this->message.sendMessage(sender, this->message.getMessage(401, sender));
}

void Server::handleChannelMessage(Client &sender, const std::string &channel, const std::string &message)
{
	if (!isExistentChannel(channel))
	{
		this->message.sendMessage(sender, this->message.getMessage(403, sender));
		return ;
	}
	for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if ((*it)->getName() == channel)
		{
			std::vector<Client> channelClients = (*it)->getClients();
			std::string fullMessage = ":" + sender.getPrefix() + " PRIVMSG " + channel + " :" + message;
			for (std::vector<Client>::iterator i = channelClients.begin(); i != channelClients.end(); ++i)
			{
				if (i->getNickname() != sender.getNickname())
					send(i->getFd(), fullMessage.c_str(), fullMessage.length(), 0);
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

Client *Server::findClient(std::string nick)
{
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
        if (it->getNickname() == nick) {
            return &(*it);
        }
    }
    return NULL;
}

Client *Server::findClient(int fd)
{
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
        if (it->getFd() == fd) {
            return &(*it);
        }
    }
    return NULL;
}

/* ANTHONY ↓: */

Channel* Server::getChannel(const std::string &channelName)
{
	// Busca el canal por nombre
	for (size_t i = 0; i < channels.size(); ++i) // o poner ++i
	{
		if (channels[i]->getName() == channelName)
			return channels[i]; // Devuelve el puntero al canal si se encuentra
	}
	return NULL; // Retorna NULL si no se encuentra el canal
}

void Server::removeChannel(const std::string &channelName)
{
	for (size_t i = 0; i < channels.size(); i++)// o poner ++i
	{
		if (channels[i]->getName() == channelName)
		{
			channels.erase(channels.begin() + i); // Elimina el canal del servidor
			break;
		}
	}
}

std::string Server::getServerName() const
{
	return serverName; // `serverName` sería un atributo de la clase Server
}

const std::vector<Client> Server::getClients() const
{
	return (clients);
}
