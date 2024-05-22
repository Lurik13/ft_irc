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
	this->_ipv4.sin_addr.s_addr = addr;
	this->_ipv4.sin_family = family;
	this->_ipv4.sin_port = port;
    this->_socket = socket(family, SOCK_STREAM, 0);
    if (this->_socket < 0)
        throw Error::Exception("Error: socket!");
	pollfd	fd;
	fd.fd = this->_socket;
	fd.events = POLLIN;
	fd.revents = 0;
	this->_fds.push_back(fd);
}

Socket::~Socket(void)
{
	std::cout << YELLOW << "Shutting down the server." << RESET << std::endl;
	for (unsigned long int i = 0; i < this->_fds.size(); i++)
		close(this->_fds[i].fd);
	// close(this->_clientSocket);
	close(this->_socket);
}

void    Socket::launch(void)
{
    std::cout << WHITE << "Launching the server ..." << RESET << std::endl;
	int status = bind(this->_socket, reinterpret_cast<struct sockaddr *>(&this->_ipv4), sizeof(this->_ipv4));
	std::cout << status;
    if (status)
	{
		std::cout << errno;
		throw Error::Exception("Error: bind!");
	}
    if (listen(this->_socket, 5))
		throw Error::Exception("Error: listen!");
}

static void	putnbr_chr(std::string s, size_t nb)
{
	size_t			n;

	n = nb;
	if (n < 0)
	{
		n = n * -1;
		s += '-';
	}
	if (n >= 10)
	{
		putnbr_chr(s, n / 10);
		putnbr_chr(s, n % 10);
	}
	else
		s += n + '0';
}

std::string	ft_itoa(int n)
{
	std::string	s = "";
;
	putnbr_chr(s, n);
	s = "103";
	return (s);
}

void sendHttpResponse(int clientSocket) {
    const char *htmlContent = "<!DOCTYPE html><html><head><title>Example Page</title></head><body><h1>Hello, World!</h1></body></html>";

    // Construire la réponse HTTP
    std::string response =
        "HTTP/1.1 200 OK\r\n"
        "Date: Tue, 21 May 2024 10:00:00 GMT\r\n"
        "Content-Type: text/html; charset=UTF-8\r\n"
        "Content-Length: " + ft_itoa(strlen(htmlContent)) + "\r\n"
        "\r\n" + htmlContent;

    // Envoyer la réponse HTTP
    send(clientSocket, response.c_str(), response.length(), 0);
}

void	Socket::acceptClient(void)
{
	pollfd	fd;
	fd.fd = accept(this->_socket, NULL, NULL);
	fd.events = POLLIN;
	fd.revents = 0;
	if (fd.fd < 0)
	{
		Socket::~Socket();
		throw Error::Exception("Error: cannot to connect a client!");
	}
	this->_fds.push_back(fd);
	std::cout << BLUE << "Client connected!" << RESET << std::endl;

	// this->_clientSocket = accept(this->_socket, NULL, NULL);
	// if (this->_clientSocket < 0)
	// 	throw Error::Exception("Error: cannot to connect a client!");
    // std::cout << BLUE << "Client connected!" << RESET << std::endl;
	// char buffer[65000] = {0};
	// memset(buffer, '\0', 65000);
	// while(recv(this->_clientSocket, buffer, 65000, 0) > 0)
	// {
	// 	std::cout << "Message from client: " << buffer << std::endl;
	// 	memset(buffer, '\0', 65000);
	// 	sendHttpResponse(this->_clientSocket);
	// }
	// std::cout << "FINISHED !" << std::endl;
}

void	Socket::handle(void)
{
	int	status;
	while (1)
	{
		status = poll(this->_fds.data(), this->_fds.size(), -1);
		if (status < 0)
		{
			Socket::~Socket();
			throw Error::Exception("Error: poll!");
		}
		if (status == 0)
			continue;
		for (unsigned long int i = 0; i < this->_fds.size(); i++)
		{
			if ((this->_fds[i].revents & POLLIN) != 1)
				continue;
			if (i == 0)
			{
				this->acceptClient();
			}
			else
			{
				char buffer[65000] = {0};
				memset(buffer, '\0', 65000);
				while(recv(this->_fds[i].fd, buffer, 65000, 0) > 0)
				{
					std::cout << "Message from client: " << buffer << std::endl;
					memset(buffer, '\0', 65000);
					sendHttpResponse(this->_fds[i].fd);
				}
				std::cout << "FINISHED !" << std::endl;
				close(this->_fds[i].fd);
				this->_fds.erase(this->_fds.begin() + i);
			}
		}
	}
}

int	Socket::getSocket(void)
{
	return (this->_socket);
}
