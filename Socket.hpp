/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 14:50:17 by lribette          #+#    #+#             */
/*   Updated: 2024/05/16 18:36:54 by lribette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "server.hpp"
#include "Error.hpp"
#include "Parse.hpp"

typedef struct infoClient
{
	struct sockaddr_in	ipv4;
	std::string		nickname;
	std::string		username;
	std::string		hostname;
	std::string		password;
	// std::string		channel;
}				infoClient;

class Socket
{
	private:
		struct sockaddr_in 			_ipv4;
		std::vector<struct pollfd>	_fds;
		std::map<int, infoClient>	_clients;
		std::string					_password;
        Socket(void);
		bool						registration(struct pollfd& fd, bool is_first);
		bool						readStdin(void);
		std::string					readClientSocket(struct pollfd& fd);
		void						parseClientInfos(std::string buffer, struct pollfd& fd);
	public:
		Socket(in_addr_t addr, in_port_t port, sa_family_t family, std::string password);
		~Socket(void);
		std::string					getPassword(void);
		void						ft_erase(struct pollfd& fd);
		void						launch(void);
		void						acceptClient(void);
		void						handle(void);
};
