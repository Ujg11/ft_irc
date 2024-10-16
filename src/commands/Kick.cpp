/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 17:52:30 by agrimald          #+#    #+#             */
/*   Updated: 2024/10/16 17:49:50 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/commands/Kick.hpp"

std::vector<std::string> Kick::SplitUsers(const std::string &userList)
{
	std::vector<std::string> users;
	std::stringstream ss(userList);
	std::string user;
	while (std::getline(ss, user, ','))
	{
		if (!user.empty())
			users.push_back(user);
	}
	return users;
}

/* FindReason: encuentra el motivo del Kick a partir de los argumentos. */
std::string Kick::FindReason(std::vector<std::string> &args)
{
	if (args.size() > 2)
	{
		std::string reason = "";
		for (size_t i = 2; i < args.size(); i++)
		{
			reason += " " + args[i]; // No redeclares la variable reason
		}
		// Si el motivo está vacío, no eliminar el primer caracter
		if (!reason.empty())
		{
			// Eliminar el espacio inicial
			reason.erase(0, 1);

			// Elimina el ':' del motivo si está presente
			if (reason[0] == ':')
				return reason.substr(1); // Elimina el primer caracter ':'
		}
		return reason;
	}
	return ""; // No se proporcionó motivo
}
/* Ejemplo  de esta funcion (FindReason):


	Si "args = {"#channel", "usuario", ":motivo del kick"}" , esta funcion
	devolveria "motivo del kick". Si no hay motivo, devuelve una cadena vacia.
*/

/* SplitChannels: Divide el primer argumento (el nombre del canal) en multiples canales
	si estan separados por comas. */
std::vector<std::string> Kick::SplitChannels(const std::string &channelList)
{
	std::vector<std::string> channels;
	std::stringstream ss(channelList);
	std::string channel;
	while (std::getline(ss, channel, ','))
	{
		if (!channel.empty())
			channels.push_back(channel);
	}
	return channels;
}
/* Ejemplo de esta funcion (SplitChannels):

	Si "argv[0] = "#channel1, #channel2", devolveria un vector con "["#channel1",
	"#channel2"]"
*/


/* Kick: Esta funcion sigue los mismos pasos descritos anteriormente para implementar
	el comando KICK. */
void Kick::execute(Server &server, Client &c, std::vector<std::string> args)
{
	// Verificar si se proporcionaron suficientes parametros
	if (args.size() < 2)
	{
		std::string cmd = "KICK";
		server.message.sendMessage(c, server.message.getMessage(461, c, cmd));
		return ;
	}
	// Obtener el canal o canales del usuario para expulsar
	std::vector<std::string> channels = SplitChannels(args[0]);
	std::vector<std::string> usersToKick = SplitUsers(args[1]);
	//std::string userToKick = args[1];
	std::string reason = FindReason(args); // Motivo opcional

	// Iterar sobre el canal
	for (size_t i = 0; i < channels.size(); i++)
	{
		std::string &channelName = channels[i];
		Channel *channel = server.getChannel(channelName);
		if (!channel)
		{
			std::string cmd = channelName;
			server.message.sendMessage(c, server.message.getMessage(403, c, cmd));
			continue;
		}
		// Verificar si el cliente que ejecuta el KICK esta en el canal
		if (!channel->isClient(c))
		{
			std::string cmd = channelName;
			server.message.sendMessage(c, server.message.getMessage(442, c, cmd));
			continue;
		}
		// Verificar si el cliente es un operador del canal
		if (!channel->isOperator(c))
		{
			std::string cmd = channelName;
			server.message.sendMessage(c, server.message.getMessage(482, c, cmd));
			continue;
		}

		for (size_t j = 0; j < usersToKick.size(); j++)
        {
			std::string &userToKick = usersToKick[j];
			Client *clientToKick = channel->getClientByName(userToKick);
			// Verificar si el usuario a expulsar esta en el canal
			if (!clientToKick)
			{
				std::string errMsg = ":LosVSCode 441 " + c.getNickname() + " " + userToKick + " " + channelName + " :They aren't on that channel\r\n"; 
				server.message.sendMessage(c, errMsg);
				continue;
			}
			// Realizar el KICK: Notificar a todos en el canal
			std::stringstream ss;
			ss << ":" << c.getNickname() << "!" << c.getUsername() << "@" << c.getIp() 
				<< " KICK " << channelName << " " << userToKick;
			if (!reason.empty())
				ss << " :" << reason;
			ss << "\r\n";
			channel->broadcast(ss.str());
			// Remover al cliente del canal
			clientToKick->removeJoinedChannel(channelName);
			channel->removeClient(clientToKick->getFd());
			
			// Si el canal queda vacio, eliminar del servidor
			if (channel->isEmpty())
			{
				server.removeChannel(channelName);
			}
		}
	}
}
/* Ejemplo de la funcion (KICK):

	Supongamos que el cliente envia el comando:

	KICK #channel usuario_a_expulsar :motivo del kick

	el vector args sera:

	args = {"#channel", "usuario_a_expulsar", ":motivo del kick"};

	1. SplitChannels("#channel") devolvera ["#channel"].
	2. FindReason(args) devolvera "motivo del kick".
	3. Verificamos:
		. Si el "channel" existe.
		. Si el cliente que ejecuta el comando es operador en ese canal.
		. Si el usuario a expulsar esta en ese canal.
	4. Si todo esta correcto, expulsamos al usuario y notificamos a todos en el canal.
*/

/* NOTA:

	. Si te da mal el la funcion execute en la linea 78
		usar este EJEMPLO:
					if (!channel)
		{
			c.sendError(ERR_NOSUCHCHANNEL, channelName);
			continue;
		}

		if (!channel->isClient(c.getFd()))
		{
			c.sendError(ERR_NOTONCHANNEL, channelName);
			continue;
		}

		if (!channel->isOperator(c.getFd()))
		{
			c.sendError(ERR_CHANOPRIVSNEEDED, channelName);
			continue;
		}

		Client *clientToKick = channel->getClientByName(userToKick);
		if (!clientToKick)
		{
			c.sendError(ERR_USERNOTINCHANNEL, channelName);
			continue;
		}
*/