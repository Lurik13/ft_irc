/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 11:31:56 by lribette          #+#    #+#             */
/*   Updated: 2024/05/30 18:04:50 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

void	pass(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients)
{
	(void)clients;
	if (parse.getArgs().size() < 1)
	{
		if (send(fd.fd, "Usage: /PASS <password>\r\n", 26, 0) < 0)
			throw Error::Exception("Error: send!");
	}
	else if (parse.getArgs().at(0) != socket.getPassword())
	{
		if (send(fd.fd, "Invalid password!\r\n", 20, 0) < 0)
			throw Error::Exception("Error: send!");
	}
	else
		socket.ft_erase(fd);
}

void	nick(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients)
{
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
	if (parse.getArgs().size() == 1 || parse.getArgs().size() == 2)
	{
		std::string toSend = "PONG " + clients[fd.fd].servername + " :" + parse.getArgs().at(0) + "\r\n";
		if (send(fd.fd, toSend.c_str(), toSend.size(), 0) < 0)
			throw Error::Exception("Error: send!");
	}
	else
	{
		std::string toSend = "PING :needs one param.\r\n";
		if (send(461, toSend.c_str(), toSend.size(), 0) < 0)
			throw Error::Exception("Error: send!");
	}
}

void    which_command(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients)
{
	size_t		i = 0;
	std::string	cmdptr[] = {"PASS", "NICK", "USER", "QUIT", "PING"};
	void		(*fxptr[])(Parse&, Socket&, struct pollfd&, std::map<int, infoClient>&) = {pass, nick, user, quit, ping};

	while (parse.getCmd() != cmdptr[i])
	{
		i++;
		if (i > cmdptr->size())
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
