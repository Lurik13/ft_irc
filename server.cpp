/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 14:45:59 by lribette          #+#    #+#             */
/*   Updated: 2024/05/16 18:37:20 by lribette         ###   ########.fr       */
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
		Socket  server(inet_addr("10.18.190.220"), htons(8080), AF_INET);
		server.launch();
	}
	catch(const std::exception& e)
	{
		std::cerr << RED << e.what() << RESET << std::endl;
	}
    return (0);
}
