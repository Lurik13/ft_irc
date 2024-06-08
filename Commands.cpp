/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 11:31:56 by lribette          #+#    #+#             */
/*   Updated: 2024/06/08 12:09:18 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

void	toSend(int fd, std::string str)
{
	std::cout << MAGENTA << str << RESET << std::endl;
	if (send(fd, str.c_str(), str.size(), 0) < 0)
		throw Error::Exception("Error: send!");
}

void	pass(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels)
{
	(void)clients;
	(void)channels;
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

void	nick(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels)
{
	(void)channels;
	if (parse.getArgs().size() != 1)
	{
		if (send(fd.fd, "Usage: /NICK <nickname>\r\n", 26, 0) < 0)
			throw Error::Exception("Error: send!");
		socket.ft_erase(fd);
	}
	else
	{
		// if (clients[fd.fd].nickname != "")
		// {
			// std::string toSend = ":" + clients[fd.fd].nickname + " NICK " + parse.getArgs().at(0) + "\r\n";
			toSend(fd.fd, ":" + clients[fd.fd].nickname + " NICK " + parse.getArgs().at(0) + "\r\n");
			// if (send(fd.fd, toSend.c_str(), toSend.size(), 0) < 0)
			// 	throw Error::Exception("Error: send!");
		// }
		clients[fd.fd].nickname = parse.getArgs().at(0);
		std::cout << "Nickname: " << clients[fd.fd].nickname << std::endl;
	}
}

void	user(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels)
{
	(void)channels;
	if (parse.getArgs().size() != 4)
	{
		toSend(fd.fd, "Usage: /USER <username> <hostname> <servername> <realname>\r\n");
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
void	quit(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels)
{
	(void)parse;
	(void)socket;
	(void)clients;
	(void)channels;
	socket.ft_erase(fd);
}

void	ping(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels)
{
	(void)parse;
	(void)socket;
	(void)clients;
	(void)channels;
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

void	join(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels)
{
	(void)socket;
	(void)clients;
	std::string 				channelName = parse.getArgs().at(0);
	std::string 				key = parse.getArgs().size() == 2 ? parse.getArgs().at(1) : "";
	std::stringstream			nameStream(channelName);
	std::stringstream			keyStream(key);
	bool						check = 0;
	std::vector<std::string>	channelNames;
	std::vector<std::string>	keys;

	if (parse.getArgs().size() == 0 || parse.getArgs().size() > 2)
	{
		toSend(fd.fd, "Usage: /JOIN <channel> {<key>}\r\n");
		return ;
	}
	while (std::getline(nameStream, channelName, ','))
		channelNames.push_back(channelName);
	while (std::getline(keyStream, key, ','))
		keys.push_back(key);
	channelName = "";
	key = "";
	for (unsigned long int i = 0; i < channelNames.size(); i++)
	{
		for (unsigned long int j = 0; j < channels.size(); j++)
		{
			// if channel exists
			if (channels.at(j).getName() == channelNames[i])
			{
				std::cout << "The channel exists" << std::endl;
				// if key is not wrong
				key = keys.size() <= i ? "" : keys[i];
				if (channels.at(j).getKey() == key)
				{
					std::cout << "Key is correct" << std::endl;
					// if client is already in the channel
					if (channels.at(j).getClients().find(fd.fd) != channels.at(j).getClients().end())
						toSend(fd.fd, "You are already in the channel\r\n");
					else
					{
						std::cout << "You have joined the channel" << std::endl;
						std::map<int, std::string>	client;
						client[fd.fd] = "@";
						channels.at(j).push(client.begin(), channelNames[i], key, "No topic is set");
						channels.push_back(channels.at(j).getChannel());
						toSend(fd.fd, "You have joined the channel " + channelNames[i] + "\r\n");
						toSend(fd.fd, channelNames[i] + " :" + channels.at(i).getTopic() + "\r\n");
						// send list of users in the channel (RPL_NAMREPLY)
						std::string	listOfUsers;
						for (std::map<int, std::string>::iterator it = channels.at(j).getClients().begin(); it != channels.at(j).getClients().end();)
						{
							listOfUsers += it->second + clients.find(it->first)->second.nickname;
							++it;
							if (it != channels.at(j).getClients().end())
								listOfUsers += " ";
						}
						toSend(fd.fd, channelNames[i] + " :" + listOfUsers + "\r\n");
						toSend(fd.fd, channelNames[i] + " :" + "End of /NAMES list.\r\n");
					}
				}
				else
				{
					std::cout << "Invalid key" << std::endl;
					toSend(fd.fd, "Invalid key.\r\n");
				}
				check = 1;
				break;
			}
		}
		// if channel exists
		if (check == 1 && i < channelNames.size())
			continue ;
		// if channel does not exist
		std::cout << "Channel does not exist" << std::endl;
		Channel	channel;
		key = keys.size() <= i ? "" : keys[i];
		std::map<int, std::string>	client;
		client[fd.fd] = "@";
		channel.push(client.begin(), channelNames[i], key, "No topic is set");
		channels.push_back(channel.getChannel());
		toSend(fd.fd, "You have joined the channel " + channel.getName() + "\r\n");
		toSend(fd.fd, channel.getName() + " :" + channel.getTopic() + "\r\n");
		// send list of users in the channel (RPL_NAMREPLY)
		std::string	listOfUsers;
		for (std::map<int, std::string>::iterator it = channel.getClients().begin(); it != channel.getClients().end();)
		{
			listOfUsers += it->second + clients.find(it->first)->second.nickname;
			++it;
			if (it != channel.getClients().end())
				listOfUsers += " ";
		}
		toSend(fd.fd, channelNames[i] + " :" + listOfUsers + "\r\n");
		toSend(fd.fd, channelNames[i] + " :" + "End of /NAMES list.\r\n");
	}
}

void	part(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels)
{
	(void)parse;
	(void)socket;
	(void)fd;
	(void)clients;
	(void)channels;
}

void	topic(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels)
{
	// (void)parse;
	(void)socket;
	(void)fd;
	(void)clients;
	(void)channels;
	// std::cout << MAGENTA << "TOPIC" << parse.getArgs().at(0) << std::endl;
	if (parse.getArgs().size() == 1 || parse.getArgs().size() == 2)
	{
		// std::string toSend = "PONG " + clients[fd.fd].servername + " :" + parse.getArgs().at(0) + "\r\n";
		toSend(fd.fd, "TOPIC " + parse.getArgs().at(0) + "\r\n");
	}
	// if (parse.getArgs())
}

void    which_command(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels)
{
	size_t		i = 0;
	std::string	cmdptr[] = {"PASS", "NICK", "USER", "QUIT", "PING", "JOIN", "PART", "TOPIC"};
	void		(*fxptr[])(Parse&, Socket&, struct pollfd&, std::map<int, infoClient>&, std::vector<class Channel>&) = {pass, nick, user, quit, ping, join, part, topic};

	while (parse.getCmd() != cmdptr[i])
	{
		i++;
		if (i > 6)
			return ;
	}
	std::cout << "Nickname: " << clients[fd.fd].nickname << std::endl;
	(*fxptr[i])(parse, socket, fd, clients, channels);
}

// CAP, NICK, USER, QUIT, PING, WHOIS, PASS, JOIN, "WHO", "PART", "LUSERS", "MOTD", "PRIVMSG"

// CAP - Négocier les capacités du client et du serveur
// PASS - Définir le mot de passe du client ✅
// NICK - Définir le pseudo du client ✅
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

