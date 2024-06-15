/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 11:31:56 by lribette          #+#    #+#             */
/*   Updated: 2024/06/15 10:01:06 by lribette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

void	pass(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels)
{
	(void)channels;
	int error = 0;

	if (parse.getArgs().size() < 1)
	{
		toSend(fd.fd, ":ft_irc.com 461 " + clients[fd.fd].nickname + " TOPIC :Not enough parameters\r\n");
		error = 1;
	}
	
	//On concatene tous les arguments au cas ou le serveur ait demande un mot de passe avec des espaces
	std::string password = getAllArgs(0, parse);
	if (password != socket.getPassword())
	{
		toSend(fd.fd, "Invalid password!\r\n");
		error = 1;
	}
	if (error == 1)
		socket.ft_erase(fd, channels);
}

// for (Channel& channel : user.channels) {
// 	for (User& channelUser : channel.users) {
// 		if (channelUser != user) {
// 			sendNotification(channelUser, oldNickname + " a changé son pseudo en " + newNickname);
// 		}
// 	}
// }
void	nick(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels)
{
	(void)channels;
	(void)socket;
	if (parse.getArgs().size() != 1)
		toSend(fd.fd, "Usage: /NICK <nickname>\r\n");
	else if (!isACorrectNickname(parse.getArgs().at(0)))
		toSend(fd.fd, ":ft_irc.com 432 " + parse.getArgs().at(0) + " :Erroneus nickname\r\n");
	else
	{
		for (std::map<int, infoClient>::iterator it = clients.begin(); it != clients.end(); ++it)
		{
			if (it->second.nickname == parse.getArgs().at(0))
			{
				toSend(fd.fd, ":ft_irc.com 433 " + it->second.nickname + " " + parse.getArgs().at(0) + " :Nickname is already in use\r\n");
				return ;
			}
		}
		toSend(fd.fd, ":" + clients[fd.fd].nickname + " NICK " + parse.getArgs().at(0) + "\r\n");
		clients[fd.fd].nickname = parse.getArgs().at(0);
		clients[fd.fd].has_a_good_nickname = 1;
	}
}

void	user(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels)
{
	(void)channels;
	if (parse.getArgs().size() != 4)
	{
		toSend(fd.fd, "Usage: /USER <username> <hostname> <servername> <realname>\r\n");
		socket.ft_erase(fd, channels);
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
	socket.ft_erase(fd, channels);
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
	std::string 						channelName = parse.getArgs().at(0);
	std::string 						key = parse.getArgs().size() == 2 ? parse.getArgs().at(1) : "";
	std::stringstream					nameStream(channelName);
	std::stringstream					keyStream(key);
	std::map<std::string, std::string>	ck;

	if (parse.getArgs().size() == 0 || parse.getArgs().size() > 2)
	{
		toSend(fd.fd, "Usage: /JOIN <channel> {<key>}\r\n");
		return ;
	}
	while (std::getline(nameStream, channelName, ','))
	{
		if (std::getline(keyStream, key, ','))
			ck[channelName] = key;
		else
			ck[channelName] = "";
	}
	for (std::map<std::string, std::string>::iterator it = ck.begin(); it != ck.end(); ++it)
	{
		std::cout << "Channel: " << it->first << " Key: " << it->second << std::endl;
		// std::map<std::string, std::string>::iterator	channel = ck.find(it->first);
		int	i = channelExists(channels, it->first);
		if (i != -1)
		{
			std::cout << "Channel exists" << std::endl;
			// if (channel is invite only)
			if (channels[i].getIsInviteOnly())
				toSend(fd.fd, ":ft_irc.com 473 " + clients[fd.fd].nickname + " " + channelName + " :Cannot join channel (+i)\r\n");
			// if key is correct
			else if (channels[i].getKey() == it->second || channels[i].getKey() == "")
			{
				std::cout << "Key is correct" << std::endl;
				// if client is already in the channel
				if (channels[i].getClients().find(fd.fd) != channels[i].getClients().end())
					toSend(fd.fd, "You are already in the channel\r\n");
				// if client is not in the channel
				else
				{
					std::cout << "You have joined the channel" << std::endl;
					// add client to the channel
					channels[i].push(fd.fd, "");
					std::string	listOfUsers;
					for (std::map<int, std::string>::iterator it = channels[i].getClients().begin(); it != channels[i].getClients().end();)
					{
						listOfUsers += it->second + clients.find(it->first)->second.nickname;
						if (it->first != fd.fd)
							toSend(it->first, ":" + clients[fd.fd].nickname + "!" + clients[fd.fd].username + "@" + clients[fd.fd].hostname + " JOIN " + channels[i].getName() + "\r\n");
						++it;
						if (it != channels[i].getClients().end())
							listOfUsers += " ";
					}
					toSend(fd.fd, ":" + clients[fd.fd].nickname + "!" + clients[fd.fd].username + "@" + clients[fd.fd].hostname + " JOIN " + channels[i].getName() + "\r\n");
					// send RPL_TOPIC
					toSend(fd.fd, ":ft_irc.com 332 " + clients[fd.fd].nickname + " " + channels[i].getName() + " :" + channels[i].getTopic() + "\r\n");
					// send list of users in the channel (RPL_NAMREPLY)
					toSend(fd.fd, ":ft_irc.com 353 " + clients[fd.fd].nickname + " = " + channels[i].getName() + " :" + listOfUsers + "\r\n");
					toSend(fd.fd, ":ft_irc.com 366 " + clients[fd.fd].nickname + " " + channels[i].getName() + " :" + "End of /NAMES list.\r\n");
				}
			}
			// if key is wrong
			else
			{
				std::cout << "Invalid key" << std::endl;
				toSend(fd.fd, ":ft_irc.com 475 " + clients[fd.fd].nickname + " " + channels[i].getName() + " :" + "Cannot join channel (+k)\r\n");
			}
		}
		else
		{
			std::cout << "Channel does not exist" << std::endl;
			// create a new channel
			Channel	c(fd.fd, it->first, it->second, "No topic is set.", "@");
			// add channel to the list of channels
			channels.push_back(c.getChannel());
			// send RPL_TOPIC
			toSend(fd.fd, ":" + clients[fd.fd].nickname + "!" + clients[fd.fd].username + "@" + clients[fd.fd].hostname + " JOIN " + c.getName() + "\r\n");
			toSend(fd.fd, ":ft_irc.com 332 " + clients[fd.fd].nickname + " " + c.getName() + " :" + c.getTopic() + "\r\n");
			// send list of users in the channel (RPL_NAMREPLY)
			toSend(fd.fd, ":ft_irc.com 353 " + clients[fd.fd].nickname + " = " + c.getName() + " :@" + clients[fd.fd].nickname + "\r\n");
			toSend(fd.fd, ":ft_irc.com 366 " + clients[fd.fd].nickname + " " + c.getName() + " :" + "End of /NAMES list.\r\n");
		}
	}
}

void	part(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels)
{
	(void)socket;

	if (parse.getArgs().size() == 0)
		toSend(fd.fd, ":ft_irc.com 461 " + clients[fd.fd].nickname + " PART :Not enough parameters\r\n");
	else
	{
		std::string					channelName = parse.getArgs().at(0);
		std::stringstream			nameStream(channelName);
		std::vector<std::string>	channelNames;

		while (std::getline(nameStream, channelName, ','))
			channelNames.push_back(channelName);
		for (unsigned long int i = 0; i < channelNames.size(); i++)
		{
			int	j = channelExists(channels, channelNames[i]);
			if (j == -1)
				toSend(fd.fd, ":ft_irc.com 403 " + clients[fd.fd].nickname + " " + channelNames[i] + " :No such channel\r\n");
			else if (channelNames[i][0] != '#')
				toSend(fd.fd, ":ft_irc.com 403 " + clients[fd.fd].nickname + " " + channelNames[i] + " :No such channel\r\n");
			else
			{
				if (!channels[j].clientIsInChannel(fd.fd))
                    toSend(fd.fd, ":ft_irc.com 442 " + clients[fd.fd].nickname + " " + channelNames[i] + " :You're not on that channel\r\n");
                else
                {
                    // FAIRE LE SYSTEME DE MODES
                    std::string reason = parse.getArgs().size() == 2 ? parse.getArgs().at(1) : "Goodbye";
                    // ENVOYER A TOUS LES AUTRES CLIENTS DU CHANNEL
                    for (std::map<int, std::string>::iterator it = channels[j].getClients().begin(); it != channels[j].getClients().end(); ++it)
                    {
                        toSend(it->first, ":" + clients[fd.fd].nickname + "!" + clients[fd.fd].username + "@" + clients[fd.fd].hostname + " PART " + channelNames[i] + " :" + reason + "\r\n");
                    }
                    channels[j].pop(fd.fd);
                    if (channels[j].isEmpty())
                        channels.erase(channels.begin() + j);
				}
			}
		}
	}
}

void	topic(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels)
{
	// 6> TOPIC #yo :t
	// 3< :zirconium.libera.chat 482 bonsoir #yo :You're not a channel operator
	// 6> TOPIC #reg :wrgiuheg rieh erih re
	// 3< :bonsoir!~lribette@81.255.67.28 TOPIC #reg :wrgiuheg rieh erih re

	(void)socket;

	if (parse.getArgs().size() == 0)
		toSend(fd.fd, ":ft_irc.com 461 " + clients[fd.fd].nickname + " TOPIC :Not enough parameters\r\n");
	else if (parse.getArgs().at(0)[0] != '#') // inutile ?
		toSend(fd.fd, ":ft_irc.com 403 " + clients[fd.fd].nickname + " " + parse.getArgs().at(0) + " :No such channel\r\n");
	else
	{
		int	i = channelExists(channels, parse.getArgs().at(0));
		if (i == -1)
			toSend(fd.fd, ":ft_irc.com 403 " + clients[fd.fd].nickname + " " + parse.getArgs().at(0) + " :No such channel\r\n");
		else
		{
			if (parse.getArgs().size() == 1 && channels[i].getTopic() == "No topic is set.")
				toSend(fd.fd, ":ft_irc.com 331 " + clients[fd.fd].nickname + " " + parse.getArgs().at(0) + " :No topic is set.\r\n");
			else if (parse.getArgs().size() > 1)
			{
				if (!channels[i].clientIsInChannel(fd.fd))
					toSend(fd.fd, ":ft_irc.com 442 " + clients[fd.fd].nickname + " " + parse.getArgs().at(0) + " :You're not on that channel\r\n");
				else
				{
					// A REVOIR
					if (channels[i].getClients().find(fd.fd)->second != "@")
						toSend(fd.fd, ":ft_irc.com 482 " + clients[fd.fd].nickname + " " + parse.getArgs().at(0) + " :You're not a channel operator\r\n");
					else
					{
						// FAIRE LE SYSTEME DE MODES
						channels[i].setTopic(getAllArgs(1, parse));
						// ENVOYER A TOUS LES AUTRES CLIENTS DU CHANNEL
						for (std::map<int, infoClient>::iterator it = clients.begin(); it != clients.end(); ++it)
						if (it->first != socket.getServerFd())
							toSend(it->first, ":" + clients[fd.fd].nickname + "!" + clients[fd.fd].username + "@" + clients[fd.fd].hostname + " TOPIC " + channels[i].getName() + " :" + channels[i].getTopic() + "\r\n");
					}
				}
			}
		}
	}
}

void	privmsg(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels)
{
	(void)socket;

	// :Alice!alice@192.168.1.10 PRIVMSG #example :Bonjour tout le monde !
	if (parse.getArgs().size() <= 1)
		toSend(fd.fd, ":ft_irc.com 461 " + clients[fd.fd].nickname + " TOPIC :Not enough parameters\r\n");
	else
	{
		std::string			targetName = parse.getArgs().at(0);
		std::stringstream	targetStream(targetName);

		while (std::getline(targetStream, targetName, ','))
		{
			if (targetName[0] == '#')
			{
				std::cout << "This is a channel" << std::endl;
				int	i = channelExists(channels, targetName);
				if (i == -1)
					toSend(fd.fd, ":ft_irc.com 403 " + clients[fd.fd].nickname + " " + targetName + " :No such channel\r\n");
				else
				{
					std::cout << "Channel exists" << std::endl;
					if (!channels[i].clientIsInChannel(fd.fd))
						toSend(fd.fd, ":ft_irc.com 442 " + clients[fd.fd].nickname + " " + targetName + " :You're not on that channel\r\n");
					else
					{
						std::string	message = getAllArgs(1, parse);
						for (std::map<int, std::string>::iterator it = channels[i].getClients().begin(); it != channels[i].getClients().end(); ++it)
						{
							toSend(it->first, ":" + clients[fd.fd].nickname + "!" + clients[fd.fd].username + "@" + clients[fd.fd].hostname + " PRIVMSG " + channels[i].getName() + " :" + message + "\r\n");
						}
					}
				}
			}
			else
			{
				std::cout << "This is a user" << std::endl;
				if (!nicknameExists(clients, targetName))
					toSend(fd.fd, ":ft_irc.com 401 " + clients[fd.fd].nickname + " " + targetName + " :No such nick/channel\r\n");
				else
				{
					for (std::map<int, infoClient>::iterator it = clients.begin(); it != clients.end(); ++it)
					{
						if (it->first != socket.getServerFd() && (it->second.nickname == targetName || it->second.nickname == clients[fd.fd].nickname))
						{
							std::string	message = getAllArgs(1, parse);
							toSend(it->first, ":" + clients[fd.fd].nickname + "!" + clients[fd.fd].username + "@" + clients[fd.fd].hostname + " PRIVMSG " + targetName + " :" + message + "\r\n");
						}
					}
				}
			}
		}
	}
}

void	invite(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels)
{
	(void)socket;
	if (parse.getArgs().size() != 2)
		toSend(fd.fd, ":ft_irc.com 461 " + clients[fd.fd].nickname + " INVITE :Not enough parameters\r\n");
	else
	{
		std::string		nickname = parse.getArgs().at(0);
		std::string		channelName = parse.getArgs().at(1);
		int				i = channelExists(channels, channelName);
		if (i == -1)
			toSend(fd.fd, ":ft_irc.com 403 " + clients[fd.fd].nickname + " " + channelName + " :No such channel\r\n");
		else
		{
			if (!channels[i].clientIsInChannel(fd.fd))
				toSend(fd.fd, ":ft_irc.com 442 " + clients[fd.fd].nickname + " " + channelName + " :You're not on that channel\r\n");
			else if (channels[i].clientIsInChannel(clients, nickname))
				toSend(fd.fd, ":ft_irc.com 443 " + clients[fd.fd].nickname + " " + nickname + " " + channelName + " :is already on channel\r\n");
			// if channel is in mode +i and if client is not an operator
			else if (!channels[i].clientIsOperator(fd.fd, clients))
				toSend(fd.fd, ":ft_irc.com 482 " + clients[fd.fd].nickname + " " + channelName + " :You're not a channel operator\r\n");
			else if (!nicknameExists(clients, nickname))
				toSend(fd.fd, ":ft_irc.com 401 " + clients[fd.fd].nickname + " " + nickname + " :No such nick/channel\r\n");
			// IF CHANNEL IS 
			else
			{
				for (std::map<int, infoClient>::iterator it = clients.begin(); it != clients.end(); ++it)
				{
					if (it->first != socket.getServerFd() && (it->second.nickname == nickname || it->second.nickname == clients[fd.fd].nickname))
						toSend(it->first, ":" + clients[fd.fd].nickname + "!" + clients[fd.fd].username + "@" + clients[fd.fd].hostname + " INVITE " + channels[i].getName() + "\r\n");
				}
			}
			toSend(fd.fd, ":ft_irc.com 341 " + clients[fd.fd].nickname + " " + nickname + " " + channelName + "\r\n");
		}
	}
}

void	kick(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels)
{
	(void)socket;
	if (parse.getArgs().size() < 2)
		toSend(fd.fd, ":ft_irc.com 461 " + clients[fd.fd].nickname + " KICK :Not enough parameters\r\n");
	else
	{
		std::string		channelName = parse.getArgs().at(0);
		std::string		nickname = parse.getArgs().at(1);
		std::string		reason = parse.getArgs().size() == 3 ? parse.getArgs().at(2) : "Goodbye";
		int				i = channelExists(channels, channelName);
		if (i == -1)
			toSend(fd.fd, ":ft_irc.com 403 " + clients[fd.fd].nickname + " " + channelName + " :No such channel\r\n");
		else
		{
			if (!channels[i].clientIsInChannel(fd.fd))
				toSend(fd.fd, ":ft_irc.com 442 " + clients[fd.fd].nickname + " " + channelName + " :You're not on that channel\r\n");
			else if (!channels[i].clientIsOperator(fd.fd, clients))
				toSend(fd.fd, ":ft_irc.com 482 " + clients[fd.fd].nickname + " " + channelName + " :You're not a channel operator\r\n");
			else if (!channels[i].clientIsInChannel(clients, nickname))
				toSend(fd.fd, ":ft_irc.com 401 " + clients[fd.fd].nickname + " " + nickname + " :No such nick/channel\r\n");
			else
			{
				for (std::map<int, infoClient>::iterator it = clients.begin(); it != clients.end(); ++it)
				{
					if (it->first != socket.getServerFd())
						toSend(it->first, ":ft_irc.com KICK " + channels[i].getName() + " " + nickname + " :" + reason + "\r\n");
					if (it->second.nickname == nickname)
						channels[i].pop(it->first);
				}
                if (channels[i].isEmpty())
                    channels.erase(channels.begin() + i);
			}
		}
	}
}

void    which_command(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels)
{
	size_t		i = 0;
	std::string	cmdptr[] = {"PASS", "NICK", "USER", "QUIT", "PING", "JOIN", "PART", "TOPIC", "PRIVMSG", "MODE", "INVITE", "KICK"};
	void		(*fxptr[])(Parse&, Socket&, struct pollfd&, std::map<int, infoClient>&, std::vector<class Channel>&) = {pass, nick, user, quit, ping, join, part, topic, privmsg, mode, invite, kick};

	while (parse.getCmd() != cmdptr[i])
	{
		i++;
		if (i > 11)
			return ;
	}
	if (hasAGoodNickname(parse, socket, fd, clients, channels, cmdptr[i]))
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
