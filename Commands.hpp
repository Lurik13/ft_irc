/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 11:37:48 by lribette          #+#    #+#             */
/*   Updated: 2024/06/14 16:44:19 by lribette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "server.hpp"
#include "Socket.hpp"
#include "Parse.hpp"
#include "Channel.hpp"

#define SIGN_MISSING 1

void		toSend(int fd, std::string errToSend);
std::string	getAllArgs(int begin, Parse parse);
int			channelExists(std::vector<class Channel>& channels, std::string channelName);
bool		isACorrectNickname(std::string name);
bool		hasAGoodNickname(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels, std::string cmd);

std::string	getMode(char mode, Channel channels, int fd);
char	    isAValidMode(std::string str);
std::string	signSorting(std::string str);
std::string	checkRelevance(std::string str, struct pollfd& fd, Channel& channel);
bool	    checkNumberOfParams(std::string str, Parse& parse, struct pollfd& fd, std::map<int, infoClient>& clients);

void		pass(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels);
void		nick(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels);
void		user(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels);
void		quit(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels);
void		join(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels);
void		part(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels);
void		topic(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels);
void	    mode(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels);
void		which_command(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels);
