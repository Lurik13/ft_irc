/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 14:50:17 by lribette          #+#    #+#             */
/*   Updated: 2024/06/04 12:22:23 by lribette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "server.hpp"
#include "Error.hpp"
#include "Parse.hpp"
#include "Channel.hpp"

typedef struct infoClient
{
	struct sockaddr_in	ipv4;
	std::string		password;
	std::string		nickname;
	std::string		username;
	std::string		servername;
	std::string		realname;
	std::string		hostname;
	std::string		mode;
	bool			is_first;
	// std::string		channel;
}				infoClient;

class Socket
{
	private:
		struct sockaddr_in 			_ipv4;
		std::vector<struct pollfd>	_fds;
		std::map<int, infoClient>	_clients;
		std::string					_password;
		std::vector<class Channel>	_channels;
        Socket(void);
		infoClient					registration(struct pollfd& fd, infoClient& client);
		bool						readStdin(void);
		std::string					readClientSocket(struct pollfd& fd);
		void						parseClientInfos(std::string buffer, struct pollfd& fd);
	public:
		Socket(in_addr_t addr, in_port_t port, sa_family_t family, std::string password);
		~Socket(void);
		std::string					getPassword(void);
		void						ft_erase(struct pollfd& fd, std::vector<class Channel>& channels);
		void						launch(void);
		void						acceptClient(void);
		void						handle(void);
};
