/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 14:51:40 by lribette          #+#    #+#             */
/*   Updated: 2024/05/16 18:39:14 by lribette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket(void) {}

Socket::Socket(in_addr_t addr, in_port_t port, sa_family_t family)
{
	this->_ipv4.sin_addr.s_addr = addr;
	this->_ipv4.sin_family = family;
	this->_ipv4.sin_port = port;
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket < 0)
        throw Error::Exception("Error: socket!");
}

Socket::~Socket(void)
{
	std::cout << YELLOW << "Shutting down the server." << RESET << std::endl;
	close(this->_clientSocket);
	close(this->_socket);
}

void    Socket::launch(void)
{
    std::cout << WHITE << "Launching the server ..." << RESET << std::endl;
    if (bind(this->_socket, reinterpret_cast<struct sockaddr *>(&this->_ipv4), sizeof(this->_ipv4)))
		throw Error::Exception("Error: bind!");
    if (listen(this->_socket, 5))
		throw Error::Exception("Error: listen!");

    // this->_clientSocket = accept(this->_socket, NULL, NULL);
	// if (this->_clientSocket < 0)
	// 	throw Error::Exception("Error: cannot to connect a client!");
    // std::cout << BLUE << "Client connected!" << RESET << std::endl;
	// char buffer[2048];
	// recv(this->_clientSocket, buffer, sizeof(buffer), 0);
	// std::cout << "Message from client: " << buffer << std::endl;
}

int	Socket::getSocket(void)
{
	return (this->_socket);
}