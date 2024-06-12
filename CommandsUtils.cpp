/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandsUtils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 11:40:11 by lribette          #+#    #+#             */
/*   Updated: 2024/06/12 18:23:50 by lribette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

void	toSend(int fd, std::string str)
{
	std::cout << MAGENTA << str << RESET << std::endl;
	if (send(fd, str.c_str(), str.size(), 0) < 0)
		throw Error::Exception("Error: send!");
}

std::string getAllArgs(int begin, Parse parse)
{
	std::string result = "";
	for (unsigned long i = begin; i < parse.getArgs().size(); i++)
	{
		result += parse.getArgs().at(i);
		if (i + 1 < parse.getArgs().size())
			result += " ";
	}
	return (result);
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
	if (clients[fd.fd].has_a_good_nickname == 0 && (cmd == "JOIN" || cmd == "PART" ||cmd == "TOPIC"))
	{
		toSend(fd.fd, ":ft_irc.com 431 " + clients[fd.fd].nickname + " :YOU HAVE TO CHANGE YOUR NICKNAME\r\n");
		return (0);
	}
	return (1);
}
