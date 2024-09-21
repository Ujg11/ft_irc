/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 17:01:10 by agrimald          #+#    #+#             */
/*   Updated: 2024/09/21 15:45:23 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Message.hpp"

# define RPL_WELCOME 1
# define RPL_YOURHOST 2
# define RPL_CREATED 3
# define RPL_MYINFO 4
# define ERR_NICKNAMEINUSE 433
# define ERR_NONICKNAMEGIVEN 431
# define ERR_ERRONEUSNICKNAME 432

// Codigos especificos para KICK y otros comandos

# define ERR_NOSUCHNICK 401
# define ERR_CANNOTSENDTOCHAN 404
# define ERR_NORECIPIENT 411
# define ERR_NOTEXTTOSEND 412

# define ERR_NOSUCHCHANNEL 403
# define ERR_NOTONCHANNEL 442
# define ERR_CHANOPRIVSNEEDED 482
# define ERR_USERNOTINCHANNEL 441


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
    std::string serverName = "Perrosl";
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

        // Errores para NICK
        case ERR_NICKNAMEINUSE:
            return ":" + serverName + " 433 " + nickName + " :Nickname is already in use\r\n";
        case ERR_NONICKNAMEGIVEN:
            return ":" + serverName + " 431 " + nickName + " :Returned when a nickname parameter expected for a command isn't found\r\n";
        case ERR_ERRONEUSNICKNAME:
            return ":" + serverName + " 431 " + nickName + " :Erroneus nickname\r\n";

        // Errores para PRIVMSG
        case ERR_NOSUCHNICK:
            return ":" + serverName + " 401 " + nickName + " :No such nick/channel\r\n";
        case ERR_CANNOTSENDTOCHAN:
            return ":" + serverName + " 404 " + nickName + " :Cannot send to channel\r\n";
        case ERR_NORECIPIENT:
            return ":" + serverName + " 411 " + nickName + " :No recipient given (PRIVMSG)\r\n";
        case ERR_NOTEXTTOSEND:
            return ":" + serverName + " 412 " + nickName + " :No text to send\r\n";

        // Errores especificos del KICK // crear channelName
        case ERR_NOSUCHCHANNEL:
            return ":" + serverName + " 403 " + nickName + " " + serverName + " :No such channel\r\n";
        case ERR_NOTONCHANNEL:
            return ":" + serverName + " 442 " + nickName + " " + serverName + " :You're not on taht channel\r\n";
        case ERR_CHANOPRIVSNEEDED:
            return ":" + serverName + " 482 " + nickName + " " + serverName + " :You are not channel operator\r\n";
        case ERR_USERNOTINCHANNEL:
            return ":" + serverName + " 441 " + nickName + " " + serverName + " :They aren't on that channel\r\n";
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