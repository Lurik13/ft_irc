/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 14:51:40 by lribette          #+#    #+#             */
/*   Updated: 2024/05/16 16:15:13 by lribette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket(void){}

Socket::Socket(in_addr_t addr, in_port_t port, sa_family_t family)
{
    std::cout << WHITE << "Launching the server ..." << RESET << std::endl;
    _ipv4.sin_addr.s_addr = addr;
    _ipv4.sin_family = family;
    _ipv4.sin_port = port;
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket == 0)
        throw Error::Exception("Error: socket!");
    
}

Socket::~Socket(void)
{
	std::cout << YELLOW << "Shutting down the server." << RESET << std::endl;
    close(_socket);
}
