/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 14:45:59 by lribette          #+#    #+#             */
/*   Updated: 2024/05/16 16:15:51 by lribette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// server.c - un micro-serveur qui accepte une connexion avant de s'arrêter
#include "server.hpp"
#include "Socket.hpp"

int main(int argc, char **argv)
{
	try
	{
		if (argc != 4)
			throw Error::Exception("");
		Socket  server(htonl(INADDR_LOOPBACK), htons(8080), AF_INET);
	}
	catch(const std::exception& e)
	{
		std::cerr << RED << e.what() << RESET << std::endl;
	}
    return (0);
}
