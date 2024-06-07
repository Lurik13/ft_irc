/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 11:31:56 by lribette          #+#    #+#             */
/*   Updated: 2024/06/05 16:16:15 by lribette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

void	toSend(int fd, std::string errToSend)
{
	std::cout << MAGENTA << errToSend << RESET << std::endl;
	if (send(fd, errToSend.c_str(), errToSend.size(), 0) < 0)
		throw Error::Exception("Error: send!");
}

void	pass(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channel)
{
	(void)clients;
	(void)channel;
	int error = 0;

	if (parse.getArgs().size() < 1)
	{
		toSend(fd.fd, "Usage: /PASS <password>\r\n");
		error = 1;
	}
	
	//On concatene tous les arguments au cas ou le serveur ait demande un mot de passe avec des espaces
	std::string password = "";
	for (unsigned long i = 0; i < parse.getArgs().size(); i++)
	{
		password += parse.getArgs().at(i);
		if (i + 1 < parse.getArgs().size())
			password += " ";
	}
	if (password != socket.getPassword())
	{
		toSend(fd.fd, "Invalid password!\r\n");
		error = 1;
	}
	if (error == 1)
		socket.ft_erase(fd);
}

void	nick(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channel)
{
	(void)channel;
	if (parse.getArgs().size() != 1)
	{
		std::cout << MAGENTA << "Usage: /NICK <nickname>" << RESET << std::endl;
		if (send(fd.fd, "Usage: /NICK <nickname>\r\n", 26, 0) < 0)
			throw Error::Exception("Error: send!");
		if (clients[fd.fd].nickname == "")
			socket.ft_erase(fd);
	}
	else
	{
		if (clients[fd.fd].nickname != "")
		{
			std::string toSend = ":" + clients[fd.fd].nickname + " NICK " + parse.getArgs().at(0) + "\r\n";
			std::cout << MAGENTA << toSend << RESET << std::endl;
			if (send(fd.fd, toSend.c_str(), toSend.size(), 0) < 0)
				throw Error::Exception("Error: send!");
		}
		clients[fd.fd].nickname = parse.getArgs().at(0);
	}
}

void	user(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channel)
{
	(void)channel;
	if (parse.getArgs().size() != 4)
	{
		std::cout << MAGENTA << "Usage: /USER <username> <hostname> <servername> <realname>" << RESET << std::endl;
		send(fd.fd, "Usage: /USER <username> <hostname> <servername> <realname>\r\n", 61, 0);
		socket.ft_erase(fd);
	}
	else
	{
		clients[fd.fd].username = parse.getArgs().at(0);
		clients[fd.fd].hostname = parse.getArgs().at(1);
		clients[fd.fd].servername = parse.getArgs().at(2);
		clients[fd.fd].realname = parse.getArgs().at(3);
	}
}

// ----------------- A CORRIGER -----------------
void	quit(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channel)
{
	(void)parse;
	(void)socket;
	(void)clients;
	(void)channel;
	socket.ft_erase(fd);
}

void	ping(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channel)
{
	(void)parse;
	(void)socket;
	(void)clients;
	(void)channel;
	if (parse.getArgs().size() == 1 || parse.getArgs().size() == 2)
	{
		// std::string toSend = "PONG " + clients[fd.fd].servername + " :" + parse.getArgs().at(0) + "\r\n";
		toSend(fd.fd, "PONG " + parse.getArgs().at(0) + "\r\n");
	}
	else
	{
		toSend(fd.fd, "Usage: /PING <server>\r\n");
	}
}

void	join(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channel)
{
	(void)socket;
	(void)clients;
	std::string channelName = parse.getArgs().at(0);
	std::string key = parse.getArgs().size() == 2 ? parse.getArgs().at(1) : "";
	bool		check = 0;

	std::cout << parse.getArgs().size() << std::endl;
	if (parse.getArgs().size() == 0 || parse.getArgs().size() > 2)
	{
		toSend(fd.fd, "Usage: /JOIN <channel> {<key>}\r\n");
		return ;
	}
	std::stringstream	nameStream(channelName);
	std::stringstream	keyStream(key);

	while (std::getline(nameStream, channelName, ',') || std::getline(keyStream, key, ','))
	{
		std::cout << channelName << std::endl;
		std::cout << key << std::endl;
		for (unsigned long int i = 0; i < channel.size(); i++)
		{
			// if channel exists
			if (channel.at(i).getName() != "" && channel.at(i).getName() == channelName)
			{
				// if key is not wrong
				if (channel.at(i).getKey() == key)
				{
					std::cout << "Key is correct" << std::endl;
					// if client is already in the channel
					if (channel.at(i).getClients().find(fd.fd) != channel.at(i).getClients().end())
						toSend(fd.fd, "You are already in the channel\r\n");
					else
					{
						std::cout << "You have joined the channel" << std::endl;
						channel.at(i).push(clients.find(fd.fd), channelName, key, "No topic is set");
						toSend(fd.fd, "You have joined the channel " + channelName + "\r\n");
						toSend(fd.fd, channel.at(i).getName() + " :" + channel.at(i).getTopic() + "\r\n");
					}
				}
				else
				{
					toSend(fd.fd, "Invalid key.\r\n");
				}
				check = 1;
				break;
			}
		}
		if (check == 1)
			return ;
		// if channel does not exist
		std::cout << RED << fd.fd << RESET << std::endl;
		std::cout << "Channel does not exist" << std::endl;
		Channel	channel;
		channel.push(clients.find(fd.fd), channelName, key, "No topic is set");
		toSend(fd.fd, "You have joined the channel " + channel.getName() + "\r\n");
		toSend(fd.fd, channel.getName() + " :" + channel.getTopic() + "\r\n");
	}

	// for (unsigned long i = 0; i < parse.getArgs().size(); i++)
	// {
	// 	std::cout << parse.getArgs().at(i) << std::endl;
	// 	if (i % 2)
	// 		std::cout << "PAIR" << std::endl;
	// 	else
	// 		std::cout << "IMPAIR" << std::endl;
	// }
	
	// if (parse.getArgs().size() == 0 || parse.getArgs().size() > 2)
	// {
	// 	std::string toSend = "Usage: /JOIN <channel> {<key>}\r\n";
	// 	std::cout << MAGENTA << toSend << RESET << std::endl;
	// 	if (send(fd.fd, toSend.c_str(), toSend.size(), 0) < 0)
	// 		throw Error::Exception("Error: send!");
	// 	return ;
	// }
}

void	part(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channel)
{
	(void)parse;
	(void)socket;
	(void)fd;
	(void)clients;
	(void)channel;
}

void    which_command(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channel)
{
	size_t		i = 0;
	std::string	cmdptr[] = {"PASS", "NICK", "USER", "QUIT", "PING", "JOIN", "PART"};
	void		(*fxptr[])(Parse&, Socket&, struct pollfd&, std::map<int, infoClient>&, std::vector<class Channel>&) = {pass, nick, user, quit, ping, join, part};

	while (parse.getCmd() != cmdptr[i])
	{
		i++;
		if (i > 5)
			return ;
	}
	(*fxptr[i])(parse, socket, fd, clients, channel);
}

// CAP, NICK, USER, QUIT, PING, WHOIS, PASS, JOIN, "WHO", "PART", "LUSERS", "MOTD", "PRIVMSG"

// CAP - Négocier les capacités du client et du serveur
// PASS - Définir le mot de passe du client ✅
// NICK - Définir le pseudo du client
// USER - Définir le nom d’utilisateur du client
// MODE - Changer le mode du client
// WHOIS - Obtenir des informations sur un client
// QUIT - Déconnecter le client ✅
// PING - Vérifier la connexion du client ✅

// KICK - Ejecter un client du channel
// INVITE - Inviter un client au channel
// TOPIC - Modifier ou afficher le thème du channel
// MODE - Changer le mode du channel :
//     — i : Définir/supprimer le canal sur invitation uniquement
//     — t : Définir/supprimer les restrictions de la commande TOPIC pour les opé- rateurs de canaux
//     — k : Définir/supprimer la clé du canal (mot de passe)
//     — o : Donner/retirer le privilège de l’opérateur de canal
//     — l : Définir/supprimer la limite d’utilisateurs pour le canal

