/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 14:51:40 by lribette          #+#    #+#             */
/*   Updated: 2024/05/22 10:26:37 by lribette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket(void) {}

Socket::Socket(in_addr_t addr, in_port_t port, sa_family_t family)
{
	pollfd	fd;

	memset(&fd, 0, sizeof(fd));
	// configuration of the server
	this->_ipv4.sin_addr.s_addr = addr;
	this->_ipv4.sin_family = family;
	this->_ipv4.sin_port = port;
	// creation of the socket (socket is a file descriptor)
	// pollfd configuration structure for multiple non-blocking file descriptors
    fd.fd = socket(family, SOCK_STREAM, 0);
	fd.events = POLLIN;
	fd.revents = 0;
    if (fd.fd < 0)
        throw Error::Exception("Error: socket!");
	this->_fds.push_back(fd);
	this->_clients[fd.fd] = this->_ipv4;
	// ---- ( TEMPORAIRE ) fd2 : stdin ----
	pollfd	fd2;
	fd2.fd = 0;
	fd2.events = POLLIN;
	fd2.revents = 0;
	this->_fds.push_back(fd2);
	// -----------------------------------
}

Socket::~Socket(void)
{
	std::cout << YELLOW << "Shutting down the server." << RESET << std::endl;
	// close all clients
	for (std::map<int, sockaddr_in>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
		close(it->first);
}

void    Socket::launch(void)
{
    std::cout << WHITE << "Launching the server..." << RESET << std::endl;
	// bind the socket to the address and port
	int status = bind(this->_fds[0].fd, reinterpret_cast<struct sockaddr *>(&this->_ipv4), sizeof(this->_ipv4));
    if (status)
		throw Error::Exception("Error: bind!");
	// listen for incoming connections
    if (listen(this->_fds[0].fd, 5))
		throw Error::Exception("Error: listen!");
}

void	Socket::acceptClient(void)
{
	pollfd		fd;
	// csin is the client's address
	sockaddr_in	csin;
	socklen_t	csin_len = sizeof(csin);
	// str is the client's address in string format
	char		str[INET_ADDRSTRLEN];

	memset(&fd, 0, sizeof(fd));
	memset(&csin, 0, sizeof(csin));
	// accept the client when it connects
	fd.fd = accept(this->_fds[0].fd, reinterpret_cast<sockaddr *>(&csin), &csin_len);
	fd.events = POLLIN;
	fd.revents = 0;
	if (fd.fd < 0)
		throw Error::Exception("Error: cannot to connect the client!");
	this->_fds.push_back(fd);
	this->_clients[fd.fd] = csin;
	// inet_ntop converts the network address to a string
	inet_ntop(AF_INET, &csin.sin_addr.s_addr, str, sizeof(str));
	std::cout << BLUE << str << " connected!" << RESET << std::endl;
}

void	Socket::handle(void)
{
	int		status = 0;
	bool	running = true;

	while (running)
	{
		status = poll(this->_fds.data(), this->_fds.size(), -1);
		if (status < 0)
			throw Error::Exception("Error: poll!");
		// if status == 0, any file descriptor has been modified
		if (status == 0)
			continue;
		for (unsigned long int i = 0; i < this->_fds.size(); i++)
		{
			// if revents is not POLLIN, the file descriptor has not been modified
			if ((this->_fds[i].revents & POLLIN) != 1)
				continue;
			// i == 0 : fd server has been modified (is ready to accept a client)
			if (i == 0)
				this->acceptClient();
			// ---- ( TEMPORAIRE ) i == 1 : fd stdin ----
			// if i == 1, the standard input has been modified (is ready to read)
			else if (i == 1)
			{
				char buffer[65000] = {0};
				memset(buffer, '\0', 65000);
				read(0, buffer, sizeof(buffer));
				if (strcmp(buffer, "exit\n") == 0)
				{
					running = false;
					break ;
				}
			}
			// -------------------------------------------
			// i > 1 : fd client has been modified
			else
			{
				char	buffer[65000] = {0};
				int		bytes = 0;
				// inet_ntop converts the network address to a string
				char	str[INET_ADDRSTRLEN];

				inet_ntop(AF_INET, &this->_clients[this->_fds[i].fd].sin_addr.s_addr, str, sizeof(str));
				memset(buffer, '\0', 65000);
				// recv receives a message from the client
				bytes = recv(this->_fds[i].fd, buffer, 65000, 0);
				if (bytes <= 0)
				{
					if (bytes == 0)
						std::cout << BLUE << str << " disconnected!" << RESET << std::endl;
					else
						std::cout << "Error: recv!" << std::endl;
					// close the client file descriptor and remove it from the map and vector
					this->_clients.erase(this->_fds[i].fd);
					close(this->_fds[i].fd);
					this->_fds.erase(this->_fds.begin() + i);
				}
				else
					std::cout << str << ": " << buffer << std::endl;
			}
		}
	}
}
