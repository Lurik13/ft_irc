/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 14:45:59 by lribette          #+#    #+#             */
/*   Updated: 2024/05/16 20:15:11 by lribette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// server.c - un micro-serveur qui accepte une connexion avant de s'arrêter
#include "server.hpp"
#include "Socket.hpp"

int main(int argc, char **argv)
{
	(void)argv;
	try
	{
		if (argc != 3)
			throw Error::Exception("Error : Syntax!\n./ircserv <port> <password>");
		Socket  server(INADDR_ANY, htons(8080), PF_INET);
		server.launch();
		server.handleClient();
	}
	catch(const std::exception& e)
	{
		std::cerr << RED << e.what() << RESET << std::endl;
	}
    return (0);
}
