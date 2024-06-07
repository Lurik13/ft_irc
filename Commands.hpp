/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 11:37:48 by lribette          #+#    #+#             */
/*   Updated: 2024/06/07 18:32:03 by lribette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "server.hpp"
#include "Socket.hpp"
#include "Parse.hpp"
#include "Channel.hpp"

void    toSend(int fd, std::string errToSend);
void	pass(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient> clients, std::vector<class Channel>& channels);
void	nick(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient> clients, std::vector<class Channel>& channels);
void	user(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient> clients, std::vector<class Channel>& channels);
void	quit(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient> clients, std::vector<class Channel>& channels);
void	join(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient> clients, std::vector<class Channel>& channels);
void	part(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient> clients, std::vector<class Channel>& channels);
void	topic(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient> clients, std::vector<class Channel>& channels);
void	which_command(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient> clients, std::vector<class Channel>& channels);
