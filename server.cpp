/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 14:45:59 by lribette          #+#    #+#             */
/*   Updated: 2024/05/17 11:48:14 by lribette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// server.c - un micro-serveur qui accepte une connexion avant de s'arrêter
#include "server.hpp"
#include "Socket.hpp"

void verif_command(int argc, char *port, char *password)
{
	if (argc != 3)
		throw Error::Exception("Syntax error!\n./ircserv <port> <password>");
	for (int i = 0; port[i]; i++)
	{
		if (!(isdigit(port[i]) || (i == 0 && port[i] == '-' && port[i + 1])))
			throw Error::Exception("Error: The port must be an integer!");
		if (i >= 4)
			throw Error::Exception("Error: Invalid port!");
	}
	if (std::atoi(port) < 0 || std::atoi(port) > 32767)
		throw Error::Exception("Error: Out of range port!");
	if ((std::string)password != PASSWORD)
		throw Error::Exception("Error: Invalid password!");
}

int main(int argc, char **argv)
{
	try
	{
		verif_command(argc, argv[1], argv[2]);
		Socket  server(inet_addr("192.168.1.35"), htons(8080), PF_INET);
		server.launch();
		server.handleClient();
	}
	catch(const std::exception& e)
	{
		std::cerr << RED << e.what() << RESET << std::endl;
	}
    return (0);
}
