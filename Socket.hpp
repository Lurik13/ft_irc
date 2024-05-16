/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 14:50:17 by lribette          #+#    #+#             */
/*   Updated: 2024/05/16 18:36:44 by lribette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "server.hpp"
#include "Error.hpp"

class Socket
{
	private:
		struct sockaddr_in  _ipv4;
        int                 _socket;
		int					_clientSocket;
	public:
        Socket(void);
		Socket(in_addr_t addr, in_port_t port, sa_family_t family);
		~Socket(void);
		void				launch(void);
		//getter
		int					getSocket(void);

};

