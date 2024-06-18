/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 11:37:48 by lribette          #+#    #+#             */
/*   Updated: 2024/06/18 17:13:41 by lribette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "server.hpp"
#include "Socket.hpp"
#include "Parse.hpp"
#include "Channel.hpp"

#define SIGN_MISSING 1

/* Commands */
void		pass(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels);
void		nick(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels);
void		user(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels);
void		quit(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels);
void		join(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels);
void		part(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels);
void		topic(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels);
void		mode(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels);
void		which_command(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels);

/* CommandsUtils.cpp */
void		toSend(int fd, std::string errToSend);
int			channelExists(std::vector<class Channel>& channels, std::string channelName);
bool		isACorrectNickname(std::string name);
bool		hasAGoodNickname(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels, std::string cmd);
bool        nicknameExists(std::map<int, infoClient>& clients, std::string targetName);
std::string	getCompleteName(struct pollfd fd, std::map<int, infoClient> clients);
bool	    checkInvitesAndLimit(struct pollfd& fd, std::map<int, infoClient>& clients, Channel& channel, std::string channelName);

/* mode.cpp */
char		isAValidMode(std::string str);
std::string	removeUselessOccurrences(std::string str, size_t i);
std::string	checkRelevance(std::string str, size_t i, int fd, Channel& channel);
std::string	removeSigns(std::string str);
std::string	sortModes(std::string str, int fd, Channel& channel);
bool		checkNumberOfParams(std::string str, Parse& parse, struct pollfd& fd, std::map<int, infoClient>& clients);
bool		checkModeArgs(std::string str, Parse& parse, struct pollfd& fd, std::map<int, infoClient>& clients, Channel& channel);
void	    executeModes(std::string str, Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, Channel& channel);
