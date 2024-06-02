/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 11:37:48 by lribette          #+#    #+#             */
/*   Updated: 2024/05/29 11:37:48 by lribette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "server.hpp"
#include "Socket.hpp"
#include "Parse.hpp"

void	pass(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients);
void	nick(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients);
void	user(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients);
void	quit(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients);
void	join(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients);
void	part(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients);
void    which_command(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients);
