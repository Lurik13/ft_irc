/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 11:31:56 by lribette          #+#    #+#             */
/*   Updated: 2024/05/29 17:21:01 by lribette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

void	pass(Parse& parse, Socket& socket, struct pollfd& fd)
{
	int error = 0;
	if (parse.getArgs().size() != 1)
	{
		send(fd.fd, "Usage: /PASS <password>\r\n", 26, 0);
		error = 1;
	}
	else if (parse.getArgs().at(0) != socket.getPassword())
	{
		send(fd.fd, "Invalid password!\r\n", 20, 0);
		error = 1;
	}
	if (error == 1)
		socket.ft_erase(fd.fd);
}

void    which_command(Parse& parse, Socket& socket, struct pollfd& fd)
{
	void    (*fxptr[])(Parse &, Socket &, pollfd &) = {pass};
	std::string cmdptr[] = {"PASS"};

	int     i = 0;
	while (parse.getCmd() != cmdptr[i])
	{
		if (i == 1)
		{
			std::cout << "Command not found!\r\n";
			return;
		}
		i++;
	}
	(*fxptr[i])(parse, socket, fd);
}

// CAP, NICK, USER, QUIT, PING, PONG, WHOIS, PASS, JOIN, "WHO", "PART", "LUSERS", "MOTD", "PRIVMSG"

// CAP - Négocier les capacités du client et du serveur
// PASS - Définir le mot de passe du client
// NICK - Définir le pseudo du client
// USER - Définir le nom d’utilisateur du client
// MODE - Changer le mode du client
// WHOIS - Obtenir des informations sur un client
// QUIT - Déconnecter le client
// PING - Vérifier la connexion du client
// PONG - Répondre à la commande PING

// KICK - Ejecter un client du channel
// INVITE - Inviter un client au channel
// TOPIC - Modifier ou afficher le thème du channel
// MODE - Changer le mode du channel :
//     — i : Définir/supprimer le canal sur invitation uniquement
//     — t : Définir/supprimer les restrictions de la commande TOPIC pour les opé- rateurs de canaux
//     — k : Définir/supprimer la clé du canal (mot de passe)
//     — o : Donner/retirer le privilège de l’opérateur de canal
//     — l : Définir/supprimer la limite d’utilisateurs pour le canal
