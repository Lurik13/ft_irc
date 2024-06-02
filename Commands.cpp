/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 11:31:56 by lribette          #+#    #+#             */
/*   Updated: 2024/05/31 17:00:20 by lribette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

void	pass(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients)
{
	(void)clients;
	int error = 0;

	if (parse.getArgs().size() < 1)
	{
		if (send(fd.fd, "Usage: /PASS <password>\r\n", 26, 0) < 0)
			throw Error::Exception("Error: send!");
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
		if (send(fd.fd, "Invalid password!\r\n", 20, 0) < 0)
			throw Error::Exception("Error: send!");
		error = 1;
	}
	if (error == 1)
		socket.ft_erase(fd);
}

void	nick(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients)
{
	std::cout << "Je passe nick\n";
	if (parse.getArgs().size() != 1)
	{
		if (send(fd.fd, "Usage: /NICK <nickname>\r\n", 26, 0) < 0)
			throw Error::Exception("Error: send!");
		socket.ft_erase(fd);
	}
	else
	{
		clients[fd.fd].nickname = parse.getArgs().at(0);
	}
}

void	user(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients)
{
	std::cout << "Je passe user\n";
	if (parse.getArgs().size() != 4)
	{
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
void	quit(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients)
{
	(void)parse;
	(void)socket;
	(void)clients;
	(void)fd;
	// socket.ft_erase(fd);
}

void	ping(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients)
{
	(void)parse;
	(void)socket;
	(void)clients;
	std::cout << "Je passe ping\n";
	if (parse.getArgs().size() == 1 || parse.getArgs().size() == 2)
	{
		// std::string toSend = "PONG " + clients[fd.fd].servername + " :" + parse.getArgs().at(0) + "\r\n";
		std::string toSend = "PONG " + parse.getArgs().at(0) + "\r\n";
		if (send(fd.fd, toSend.c_str(), toSend.size(), 0) < 0)
			throw Error::Exception("Error: send!");
		std::cout << toSend;
	}
	else
	{
		std::string toSend = "PING :needs one param.\r\n";
		if (send(fd.fd, toSend.c_str(), toSend.size(), 0) < 0)
			throw Error::Exception("Error: send!");
	}
}

void	join(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients)
{
	(void)socket;
	std::cout << "Je passe join\n";
	if (parse.getArgs().size() == 1)
	{
		std::string toSend = ":" + clients[fd.fd].nickname + "!" + clients[fd.fd].username + "@" + clients[fd.fd].hostname + " JOIN " + parse.getArgs().at(0) + "\r\n";
		if (send(fd.fd, toSend.c_str(), toSend.size(), 0) < 0)
			throw Error::Exception("Error: send!");
	}
	else
	{
		std::string toSend = "JOIN :needs one param.\r\n";
		if (send(fd.fd, toSend.c_str(), toSend.size(), 0) < 0)
			throw Error::Exception("Error: send!");
	}
}

void	part(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients)
{
	(void)socket;
	std::cout << "Je passe part" << std::endl;
	if (parse.getArgs().size() == 1)
	{
		std::string toSend = ":" + clients[fd.fd].nickname + "!" + clients[fd.fd].username + "@" + clients[fd.fd].hostname + " PART " + parse.getArgs().at(0) + "\r\n";
		if (send(fd.fd, toSend.c_str(), toSend.size(), 0) < 0)
			throw Error::Exception("Error: send!");
	}
	else
	{
		std::string toSend = "PART :needs one param.\r\n";
		if (send(fd.fd, toSend.c_str(), toSend.size(), 0) < 0)
			throw Error::Exception("Error: send!");
	}
}

void    which_command(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients)
{
	size_t		i = 0;
	std::string	cmdptr[] = {"PASS", "NICK", "USER", "QUIT", "PING", "JOIN", "PART"};
	void		(*fxptr[])(Parse&, Socket&, struct pollfd&, std::map<int, infoClient>&) = {pass, nick, user, quit, ping, join, part};

	while (parse.getCmd() != cmdptr[i])
	{
		i++;
		if (i > 5)
			return ;
	}
	(*fxptr[i])(parse, socket, fd, clients);
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