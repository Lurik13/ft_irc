/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandsUtils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 11:40:11 by lribette          #+#    #+#             */
/*   Updated: 2024/06/18 17:09:31 by lribette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

void	toSend(int fd, std::string str)
{
	std::cout << MAGENTA << str << RESET << std::endl;
	send(fd, str.c_str(), str.size(), MSG_NOSIGNAL);
}

int	channelExists(std::vector<class Channel>& channels, std::string channelName)
{
	for (unsigned long int i = 0; i < channels.size(); i++)
	{
		if (channels.at(i).getName() == channelName)
			return (i);
	}
	return (-1);
}

bool	isACorrectNickname(std::string name)
{
	if (name.size() == 0 || name.size() > 30)
		return (0);
	std::string allowed = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789|_{}[]";
	for (int i = 0; name[i]; i++)
	{
		if (allowed.find(name[i]) == std::string::npos)
			return (0);
	}
	return (1);
}

bool	hasAGoodNickname(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels, std::string cmd)
{
	(void)parse;
	(void)socket;
	(void)channels;
	
	if (clients[fd.fd].has_a_good_nickname == 0 && (cmd == "JOIN" || cmd == "PART" || cmd == "TOPIC" || cmd == "PRIVMSG" || cmd == "MODE" || cmd == "INVITE" || cmd == "KICK"))
	{
		toSend(fd.fd, ":ft_irc.com 431 " + clients[fd.fd].nickname + " :YOU HAVE TO CHANGE YOUR NICKNAME\r\n");
		return (0);
	}
	return (1);
}

bool	nicknameExists(std::map<int, infoClient>& clients, std::string targetName)
{
	for (std::map<int, infoClient>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->second.nickname == targetName)
			return (true);
	}
	return (false);
}

std::string	getCompleteName(struct pollfd fd, std::map<int, infoClient> clients)
{
	return (":" + clients[fd.fd].nickname + "!" + clients[fd.fd].username + "@" + clients[fd.fd].hostname);
}

bool	checkInvitesAndLimit(struct pollfd& fd, std::map<int, infoClient>& clients, Channel& channel, std::string channelName)
{
	if (channel.getIsInviteOnly())
	{
		bool inviteFound = false;
		for (size_t invitesLen = 0; invitesLen < clients[fd.fd].channelsInvitingMe.size(); invitesLen++)
		{
			if (clients[fd.fd].channelsInvitingMe.at(invitesLen) == channelName)
			{
				clients[fd.fd].channelsInvitingMe.erase(clients[fd.fd].channelsInvitingMe.begin()+invitesLen);
				inviteFound = true;
			}
		}
		if (inviteFound == false)
		{
			toSend(fd.fd, ":ft_irc.com 473 " + clients[fd.fd].nickname + " " + channelName + " :Cannot join channel (+i)\r\n");
			return (EXIT_FAILURE);
		}
	}
	if (channel.getMode('l', fd.fd) == "+l")
	{
		if (channel.getClientsNumber() >= channel.getNbMaxOfClients())
		{
			toSend(fd.fd, ":ft_irc.com 471 " + clients[fd.fd].nickname + " " + channelName + " :Cannot join channel (+l)\r\n");
			return (EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
}
