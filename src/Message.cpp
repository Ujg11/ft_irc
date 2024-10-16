/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 17:01:10 by agrimald          #+#    #+#             */
/*   Updated: 2024/10/16 15:42:16 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../inc/Message.hpp"

# define RPL_WELCOME 1
# define RPL_YOURHOST 2
# define RPL_CREATED 3
# define RPL_MYINFO 4
# define ERR_NICKNAMEINUSE 433
# define ERR_NONICKNAMEGIVEN 431
# define ERR_ERRONEUSNICKNAME 432
# define ERR_UNKNOWNCOMMAND 421

// Codigos especificos para KICK y otros comandos

# define ERR_NOSUCHNICK 401
# define ERR_CANNOTSENDTOCHAN 404
# define ERR_NORECIPIENT 411
# define ERR_NOTEXTTOSEND 412

# define ERR_NEEDMOREPARAMS 461
# define ERR_USERONCHANNEL 443

# define ERR_NOSUCHCHANNEL 403
# define ERR_NOTONCHANNEL 442
# define ERR_CHANOPRIVSNEEDED 482
# define ERR_USERNOTINCHANNEL 441
# define ERR_BADCHANNELKEY 475
# define ERR_CHANNELISFULL 471
# define ERR_INVITEONLYCHAN 473

// Codigos especificos para el Mode

# define RPL_CHANNELMODEIS 324


std::string Message::timeNow() const //funcion que es para el server y guardase el resultado en una variable
{
	//obtener el tiempo actual
	std::time_t current_time = std::time(NULL);
	//convertirlo a una cadena de texto legible;
	std::string time_str = std::ctime(&current_time);

	//Eliminar el salto de linea que ctime agrega
	if (!time_str.empty())
	{
		time_str.erase(time_str.length() - 1);
	}
	return time_str;
}

////funcion de agarrar el mensaje  para luego enviar el mensaje con sendmessage
std::string Message::getMessage(int code, Client &client) const
{
	std::string nickName = client.getNickname();
	std::string userName = client.getUsername();
	std::string serverName = "LosVSCode";
	std::string serverVersion = "42";
	std::string dateTime = timeNow();

	switch (code)
	{
		case RPL_WELCOME:
			return ":" + serverName + " 001 " + nickName + " :Welcome to the Internet Relay Network " + client.getPrefix() + "\r\n";
		case RPL_YOURHOST:
			return ":" + serverName + " 002 " + nickName + " :Your host is " + serverName + ", running version " + serverVersion + "\r\n";
		case RPL_CREATED:
			return ":" + serverName + " 003 " + nickName + " :This server was created at " + dateTime + "\r\n";
		case RPL_MYINFO:
			return ":" + serverName + " 004 " + nickName + " :" + serverName + " " + serverVersion + " " + "\r\n"; // con tal que envie el mensaje al cliente, ta bien
		case ERR_UNKNOWNCOMMAND:
			return ":" + serverName + " 421 " + nickName + " FOOBAR :Unknown command\r\n";
		
		// Errores para NICK
		case ERR_NICKNAMEINUSE:
			return ":" + serverName + " 433 " + nickName + " :Nickname is already in use\r\n";
		case ERR_NONICKNAMEGIVEN:
			return ":" + serverName + " 431 " + nickName + " :Nickname isn't found\r\n";
		case ERR_ERRONEUSNICKNAME:
			return ":" + serverName + " 432 " + nickName + " :Erroneus nickname\r\n";

		// Errores para PRIVMSG
		case ERR_NOSUCHNICK:
			return ":" + serverName + " 401 " + nickName + " :No such nick/channel\r\n";
		case ERR_NORECIPIENT:
			return ":" + serverName + " 411 " + nickName + " :No recipient given (PRIVMSG)\r\n";
		case ERR_NOTEXTTOSEND:
			return ":" + serverName + " 412 " + nickName + " :No text to send\r\n";

		// Errores especificos del KICK // crear channelName
		case ERR_NOSUCHCHANNEL:
			return ":" + serverName + " 403 " + nickName + " " + serverName + " :No such channel\r\n";
		case ERR_NOTONCHANNEL:
			return ":" + serverName + " 442 " + nickName + " " + serverName + " :You're not on taht channel\r\n";
		
		case ERR_USERNOTINCHANNEL:
			return ":" + serverName + " 441 " + nickName + " " + serverName + " :They aren't on that channel\r\n";
		
		// Errores especificos para el Mode
		case RPL_CHANNELMODEIS:
			return ":" + serverName + " 324 "+  nickName + " " + serverName +  "PONER ALGO AQUI\r\n";
		default:
			return "NO RESPONSE FOUND";
	}
}

std::string Message::getMessage(int code, Client &client, std::string &cmd) const
{
	std::string nickName = client.getNickname();
	std::string userName = client.getUsername();
	std::string serverName = "LosVSCode";
	std::string serverVersion = "42";
	std::string dateTime = timeNow();

	switch (code)
	{
		case ERR_NEEDMOREPARAMS:
			return ":" + serverName + " 461 " + nickName + " " + cmd + " :Not enough parameters\r\n";
		case ERR_USERONCHANNEL:
			return ":" + serverName + " 443 " + nickName + " " + cmd + " :Client is already in the Channel\r\n";
		case ERR_NOSUCHCHANNEL:
			return ":" + serverName + " 403 " + nickName + " " + cmd + " :No such channel\r\n";
		case ERR_CANNOTSENDTOCHAN:
			return ":" + serverName + " 404 " + nickName + " " + cmd + " :Cannot send to channel\r\n";
		case ERR_BADCHANNELKEY:
			return ":" + serverName + " 475 " + nickName + " " + cmd + " :Cannot join channel (+k)\r\n";
		case ERR_CHANNELISFULL:
			return ":" + serverName + " 471 " + nickName + " " + cmd + " :Cannot join channel (+l)\r\n";
		case ERR_INVITEONLYCHAN:
			return ":" + serverName + " 473 " + nickName + " " + cmd + " :Cannot join channel (+i)\r\n";
		case ERR_CHANOPRIVSNEEDED:
			return ":" + serverName + " 482 " + nickName + " " + cmd + " :You are not channel operator\r\n";
		default:
			return "NO RESPONSE FOUND";
	}
	
}

//funcion sendmessage
void Message::sendMessage(const Client &client, std::string message)
{
	if (message.empty())
		return ;
	send(client.getFd(), message.c_str(), message.length(), MSG_DONTWAIT); //si falla, hacer un if con -1;
}

void Message::sendHandShake(Client &c)
{
	sendMessage(c, getMessage(1, c));
	sendMessage(c, getMessage(2, c));
	sendMessage(c, getMessage(3, c));
	sendMessage(c, getMessage(4, c));
}