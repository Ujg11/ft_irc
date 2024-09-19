/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 17:01:10 by agrimald          #+#    #+#             */
/*   Updated: 2024/09/18 16:54:58 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Message.hpp"

# define RPL_WELCOME 1
# define RPL_YOURHOST 2
# define RPL_CREATED 3
# define RPL_MYINFO 4
# define ERR_NICKNAMEINUSE 433

// Codigos especificos para KICK y otros comandos

# define ERR_NOSUCHCHANNEL 403
# define ERR_NOTONCHANNEL 442
# define ERR_CHANOPRIVSNEEDED 482
# define ERR_USERNOTINCHANNEL 441
# define ERR_NICKNAMEINUSE 433

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
            return ":" + serverName + " 002 " + nickName + " :Your host is <servername>, running version " + serverVersion + "\r\n";
        case RPL_CREATED:
            return ":" + serverName + " 003 " + nickName + " :This server was created at " + dateTime + "\r\n";
        case RPL_MYINFO:
            return ":" + serverName + " 004 " + nickName + " :" + serverName + " " + serverVersion + " " + "\r\n"; // con tal que envie el mensaje al cliente, ta bien

        case ERR_NICKNAMEINUSE: 433
            return ":" + serverName + " 433 " + nickName + " :Nickname is already in use\r\n";

        // Errores especificos del KICK
        case ERR_NOSUCHCHANNEL:
            return ":" + serverName + " 403 " + nickName + " " + channelName + " :No such channel\r\n";
        case ERR_NOTONCHANNEL:
            return ":" + serverName + " 442 " + nickName + " " + channelName + " :You're not on taht channel\r\n";
        case ERR_CHANOPRIVSNEEDED:
            return ":" + serverName + " 482 " + nickName + " " + channelName + " :You are not channel operator\r\n";
        case ERR_USERNOTINCHANNEL:
            return ":" + serverName + " 441 " + nickName + " " + channelName + " :They aren't on that channel\r\n";
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