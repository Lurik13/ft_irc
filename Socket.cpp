/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 14:51:40 by lribette          #+#    #+#             */
/*   Updated: 2024/05/30 17:17:54 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"
#include "Commands.hpp"

// Définir l'opérateur d'égalité pour la structure pollfd
bool operator==(const pollfd& lhs, const pollfd& rhs) {
    return lhs.fd == rhs.fd && lhs.events == rhs.events && lhs.revents == rhs.revents;
}

std::string	Socket::getPassword() {return this->_password;}

void	Socket::ft_erase(struct pollfd& fd)
{
	std::vector<struct pollfd>::iterator	it = std::find(this->_fds.begin(), this->_fds.end(), fd);

	std::cout << BLUE << fd.fd << " disconnected!" << RESET << std::endl;
	this->_clients.erase(fd.fd);
	close(fd.fd);
	this->_fds.erase(it);
}

Socket::Socket(void) {}

Socket::Socket(in_addr_t addr, in_port_t port, sa_family_t family, std::string password)
{
	pollfd	fd;

	memset(&fd, 0, sizeof(fd));
	// configuration of the server
	this->_ipv4.sin_addr.s_addr = addr;
	this->_ipv4.sin_family = family;
	this->_ipv4.sin_port = port;
	this->_password = password;
	// creation of the socket (socket is a file descriptor)
	// pollfd configuration structure for multiple non-blocking file descriptors
    fd.fd = socket(family, SOCK_STREAM, 0);
	fd.events = POLLIN;
	fd.revents = 0;
    if (fd.fd < 0)
    {
        throw Error::Exception("Error: socket!");
	}
	this->_fds.push_back(fd);
	this->_clients[fd.fd].ipv4 = this->_ipv4;
	this->_clients[fd.fd].nickname = "";
	this->_clients[fd.fd].username = "";
	this->_clients[fd.fd].hostname = "";
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
	for (std::map<int, infoClient>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
		close(it->first);
}

void    Socket::launch(void)
{
    std::cout << WHITE << "Launching the server..." << RESET << std::endl;
	// bind the socket to the address and port
	int opt = 1;
	if (setsockopt(this->_fds[0].fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
			throw Error::Exception("Error: setsockopt!");
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

	memset(&fd, 0, sizeof(fd));
	memset(&csin, 0, sizeof(csin));
	// accept the client when it connects
	fd.fd = accept(this->_fds[0].fd, reinterpret_cast<sockaddr *>(&csin), &csin_len);
	fd.events = POLLIN;
	fd.revents = 0;
	if (fd.fd < 0)
		throw Error::Exception("Error: cannot to connect the client!");
	this->_fds.push_back(fd);
	this->_clients[fd.fd].ipv4 = csin;
	this->_clients[fd.fd].nickname = "";
	this->_clients[fd.fd].username = "";
	this->_clients[fd.fd].hostname = "";
	this->_clients[fd.fd].password = "";
	this->_clients[fd.fd].is_first = 1;
	std::cout << GREEN << fd.fd << " connected!" << RESET << std::endl;
}

infoClient	Socket::registration(struct pollfd& fd, infoClient& client)
{
	if (client.is_first == true)
	{
		send(fd.fd, "001 bonsoir :Welcome to the ft_irc.com Network, bonsoir[!bonsoir@192.168.1.35]\r\n", 81, 0);
		send(fd.fd, "002 bonsoir :Your host is ft_irc.com, running version 1.0\r\n", 60, 0);
		send(fd.fd, "003 bonsoir :This server was created 2024/05/25 10:26:37\r\n", 59, 0);
		send(fd.fd, "004 bonsoir :There are 1 users and 0 services on 1 servers\r\n", 61, 0);
		client.is_first = false;
	}
	return (client);
}

bool	Socket::readStdin(void)
{
	char buffer[65000] = {0};

	memset(buffer, '\0', 65000);
	read(0, buffer, sizeof(buffer));
	if (strcmp(buffer, "exit\n") == 0)
		return (false);
	return (true);
}

std::string	Socket::readClientSocket(struct pollfd& fd)
{
	char	buffer[65000] = {0};
	int		bytes = 0;

	memset(buffer, '\0', 65000);
	// recv receives a message from the client
	bytes = recv(fd.fd, buffer, 65000, 0);
	if (bytes <= 0)
	{
		if (bytes != 0)
			std::cout << RED << "Error: recv!" << std::endl;
		// close the client file descriptor and remove it from the map and vector
		ft_erase(fd);
	}
	else
	{
		if (buffer[0] && strcmp(buffer, "\r\n"))
			std::cout << fd.fd << ":\n" << buffer << std::endl;
	}
	return (buffer);
}

void	Socket::parseClientInfos(std::string buffer, struct pollfd& fd)
{
	Parse		parsing;
	std::string	lines = buffer;
	std::string	line;

	unsigned long int	start = 0;
	unsigned long int	end = 0;

	while (end != std::string::npos)
	{
		end = lines.find("\r\n", start);
		line = lines.substr(start, end - start);
		if (this->_clients.find(fd.fd) != this->_clients.end())
		{
			which_command(parsing.parse(line), *this, fd, this->_clients);
		}
		start = end + 2;
	}
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
				running = this->readStdin();
			// -------------------------------------------
			// i > 1 : fd client has been modified
			else
			{
				std::string	buffer = this->readClientSocket(this->_fds[i]);
				this->parseClientInfos(buffer, this->_fds[i]);
				this->_clients[this->_fds[i].fd] = this->registration(this->_fds[i], this->_clients[this->_fds[i].fd]);
			}
		}
	}
}
