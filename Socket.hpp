/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 14:50:17 by lribette          #+#    #+#             */
/*   Updated: 2024/05/16 15:56:13 by lribette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "server.hpp"
#include "Error.hpp"

class Socket
{
	private:
		struct sockaddr_in  _ipv4;
        // struct sockaddr_in6 _ipv6;
        int                 _socket;
	public:
        Socket(void);
		Socket(in_addr_t addr, in_port_t port, sa_family_t family);
		~Socket(void);
};

