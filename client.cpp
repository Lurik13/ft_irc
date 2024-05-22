/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 18:16:33 by lribette          #+#    #+#             */
/*   Updated: 2024/05/16 18:41:13 by lribette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

int main(void)
{
	int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in serverAddress;
    std::string  line = "";

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8080);
	serverAddress.sin_addr.s_addr = inet_addr("192.168.1.35");

	connect(clientSocket, reinterpret_cast<struct sockaddr *>(&serverAddress), sizeof(serverAddress));
    std::cout << "Connected to the server!" << std::endl;
    while (line != "exit")
    {
        std::getline(std::cin, line);
        send(clientSocket, line.c_str(), sizeof(line), 0);
    }
    // char buffer[65000] = {0};
    // std::cout << "WAIT... Message from server: " << std::endl;
    // recv(clientSocket, buffer, 65000, 0);
	// std::cout << buffer << std::endl;
    close(clientSocket);
    return (0);
}
