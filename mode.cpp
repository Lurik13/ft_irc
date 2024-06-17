/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 10:42:25 by lribette          #+#    #+#             */
/*   Updated: 2024/06/17 10:13:05 by lribette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

std::string	getMode(char mode, Channel channel, int fd)
{
	std::string result = "";
	if (mode == 'i')
	{
		if (channel.getIsInviteOnly() == 0)
			return ("-i");
		return ("+i");
	}
	else if (mode == 't')
	{
		if (channel.getCanDefineTopic() == 0)
			return ("-t");
		return ("+t");
	}
	else if (mode == 'k')
	{
		if (channel.getKey() == "")
			return ("-k");
		return ("+k");
	}
	else if (mode == 'o')
	{
		if (channel.getClients()[fd] == "")
			return ("-o");
		return ("+o");
	}
	else //if (mode == 'l')
	{
		if (channel.getNbMaxOfClients() == 2147483647)
			return ("-l");
		return ("+l");
	}
}

char	isAValidMode(std::string str)
{
	std::string allowedChars = "itkol+-";
	if (str[0] != '+' && str[0] != '-')
		return (SIGN_MISSING);
	int i = 1;
	while (str[i])
	{
		if (allowedChars.find(str[i]) == std::string::npos)
			return (str[i]);
		i++;
	}
	if (str[i] == '+' || str[i] == '-')
		return (SIGN_MISSING);
	return (EXIT_SUCCESS);
}

std::string removeUselessOccurrences(std::string str, size_t i)
{
	if (i == std::string::npos)
		return(str);
	char c = str[i];

	i--;
	while (i != 0)
	{
		if (str[i] == c)
			str.erase(str.begin()+i);
		i--;
	}
	return (str);
}

std::string	checkRelevance(std::string str, size_t i, int fd, Channel& channel)
{
	if (i == std::string::npos)
		return(str);

	std::string temp = "";
	int j = i - 1;
	while (str[j] != '+' && str[j] != '-')
		j--;
	temp += str[j];
	temp += str[i];
	if (temp != "+k" && temp != "+l" && getMode(str[i], channel, fd) == temp)
		str.erase(str.begin()+i);
	return (str);
}

std::string removeSigns(std::string str)
{
	int i = 0;
	while (str[i])
	{
		if ((str[i] == '+' || str[i] == '-') && str[i + 1] && (str[i + 1] == '+' || str[i + 1] == '-'))
			str.erase(str.begin()+i);
		else
			i++;
	}
	if (str[str.size() - 1] == '+' || str[str.size() - 1] == '-')
		str.erase(str.begin()+str.size()-1);
	return (str);
}

std::string	sortModes(std::string str, int fd, Channel& channel)
{
	std::string uselessOccurrences = "itkl";

	for (int i = 0; uselessOccurrences[i]; i++)
	{
		str = removeUselessOccurrences(str, str.find_last_of(uselessOccurrences[i]));
		str = checkRelevance(str, str.find(uselessOccurrences[i]), fd, channel);
	}
	str = removeSigns(str);
	return (str);
}

std::string	whichSign(std::string str, int c)
{
	std::string result = "";
	int sign = c;
	while (str[sign] != '+' && str[sign] != '-')
		sign--;
	result += str[sign];
	result += str[c];
	return (result);
}

bool	checkNumberOfParams(std::string str, Parse& parse, struct pollfd& fd, std::map<int, infoClient>& clients)
{
	size_t nbOfParamsNeeded = 0;
	for (int i = 1; str[i]; i++)
	{
		if (str[i] == 'k' || str[i] == 'o')
			nbOfParamsNeeded++;
		else if (str[i] == 'l' && whichSign(str, i) == "+l")
			nbOfParamsNeeded++;
	}
	if (nbOfParamsNeeded != parse.getArgs().size() - 2)
	{
		toSend(fd.fd, ":ft_irc.com 461 " + clients[fd.fd].nickname + " MODE :Wrong number of parameters.\r\n");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

bool	checkModeArgs(std::string str, Parse& parse, struct pollfd& fd, std::map<int, infoClient>& clients, Channel& channel)
{
	int argIndex = 2;
	for (int i = 1; str[i]; i++)
	{
		if (str[i] == 'k')
			argIndex++;
		else if (str[i] == 'o')
		{
			if (channel.clientIsInChannel(clients, parse.getArgs().at(argIndex)) == 0)
			{
				toSend(fd.fd, ":ft_irc.com 401 " + clients[fd.fd].nickname + " " + parse.getArgs().at(argIndex) + " :No such nick.\r\n");
				return (EXIT_FAILURE);
			}
			argIndex++;
		}
		else if (str[i] == 'l' && whichSign(str, i) == "+l")
		{
			for (int j = 0; parse.getArgs().at(argIndex)[j]; j++)
			{
				if (j == 3)
				{
					toSend(fd.fd, ":ft_irc.com 696 " + clients[fd.fd].nickname + " " + channel.getName() + " l " + parse.getArgs().at(argIndex) + " :The channel limit can't exceed 999.\r\n");
					return (EXIT_FAILURE);
				}
				if (parse.getArgs().at(argIndex)[j] < '0' || parse.getArgs().at(argIndex)[j] > '9')
				{
					toSend(fd.fd, ":ft_irc.com 696 " + clients[fd.fd].nickname + " " + channel.getName() + " l " + parse.getArgs().at(argIndex) + " :The channel limit must be a positive number.\r\n");
					return (EXIT_FAILURE);
				}
			}
			argIndex++;
		}
	}
	return (EXIT_SUCCESS);
}

void	executeModes(std::string str, Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, Channel& channel)
{
	int argIndex = 2;
	for (int i = 1; str[i]; i++)
	{
		if (str[i] == 'i')
		{
			channel.setIsInviteOnly(whichSign(str, i) == "+i");
			sendToTheChannel(fd.fd, 1, channel, getCompleteName(fd, clients) + " MODE " + channel.getName() + " " + whichSign(str, i) + "\r\n");
		}
		
		else if (str[i] == 't')
		{
			channel.setCanDefineTopic(whichSign(str, i) == "+t");
			sendToTheChannel(fd.fd, 1, channel, getCompleteName(fd, clients) + " MODE " + channel.getName() + " " + whichSign(str, i) + "\r\n");
		}
		
		else if (str[i] == 'k')
		{
			if (whichSign(str, i) == "-k")
			{
				if (parse.getArgs().at(argIndex) != channel.getKey())
					toSend(fd.fd, ":ft_irc.com 525 " + clients[fd.fd].nickname + " " + channel.getName() + " :Key is not well-formed.\r\n");
				else
				{
					channel.setKey("");
					sendToTheChannel(fd.fd, 1, channel, getCompleteName(fd, clients) + " MODE " + channel.getName() + " -k\r\n");
				}
			}
			else
			{
				channel.setKey(parse.getArgs().at(argIndex));
				sendToTheChannel(fd.fd, 1, channel, getCompleteName(fd, clients) + " MODE " + channel.getName() + " +k " + parse.getArgs().at(argIndex) + "\r\n");
			}
			argIndex++;
		}
		
		else if (str[i] == 'o')
		{
			bool isOperator = channel.clientIsOperator(socket.getClientFd(parse.getArgs().at(argIndex)));
			if (whichSign(str, i) == "-o")
			{
				if (isOperator == false)
					toSend(fd.fd, ":ft_irc.com 502 " + clients[fd.fd].nickname + " :User is not an operator.\r\n");
				else
				{
					channel.setOperator(socket.getClientFd(parse.getArgs().at(argIndex)), "");
					sendToTheChannel(fd.fd, 1, channel, getCompleteName(fd, clients) + " MODE " + channel.getName() + " -o " + parse.getArgs().at(argIndex) + "\r\n");
				}
			}
			else
			{
				if (isOperator == false)
				{
					channel.setOperator(socket.getClientFd(parse.getArgs().at(argIndex)), "@");
					sendToTheChannel(fd.fd, 1, channel, getCompleteName(fd, clients) + " MODE " + channel.getName() + " +o " + parse.getArgs().at(argIndex) + "\r\n");
				}
				else
					toSend(fd.fd, ":ft_irc.com 502 " + clients[fd.fd].nickname + " :User is an operator.\r\n");
			}
			argIndex++;
		}
		
		else if (str[i] == 'l')
		{
			if (whichSign(str, i) == "-l")
			{
				channel.setLimit(2147483647);
				sendToTheChannel(fd.fd, 1, channel, getCompleteName(fd, clients) + " MODE " + channel.getName() + " -l\r\n");
			}
			else
			{
				channel.setLimit(std::atoi(parse.getArgs().at(argIndex).c_str()));
				sendToTheChannel(fd.fd, 1, channel, getCompleteName(fd, clients) + " MODE " + channel.getName() + " +l " + parse.getArgs().at(argIndex) + "\r\n");
			}
			argIndex++;
		}
	}
}

// — i : Définir/supprimer le canal sur invitation uniquement
// — t : Définir/supprimer les restrictions de la commande TOPIC pour les opérateurs de canaux
// — k : Définir/supprimer la clé du canal (mot de passe)
// — o : Donner/retirer le privilège de l’opérateur de canal
// — l : Définir/supprimer la limite d’utilisateurs pour le canal
void	mode(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels)
{
	if (parse.getArgs().size() == 0)
		toSend(fd.fd, ":ft_irc.com 461 " + clients[fd.fd].nickname + " MODE :Not enough parameters\r\n");
	else
	{
		if (parse.getArgs().at(0) == "Guest" || parse.getArgs().at(0) == clients[fd.fd].nickname)
			return ;
		int	i = channelExists(channels, parse.getArgs().at(0));
		if (i == -1)
			toSend(fd.fd, ":ft_irc.com 403 " + clients[fd.fd].nickname + " " + parse.getArgs().at(0) + " :No such channel\r\n");
		else
		{
			if (parse.getArgs().size() == 1)
			{
				std::string allModes = " " + getMode('i', channels[i], fd.fd) + getMode('t', channels[i], fd.fd) + getMode('k', channels[i], fd.fd) + getMode('o', channels[i], fd.fd) + getMode('l', channels[i], fd.fd);
				toSend(fd.fd, getCompleteName(fd, clients) + " MODE " + channels[i].getName() + allModes + "\r\n");
			}
			else if (parse.getArgs().size() > 1)
			{
				if (channels[i].clientIsOperator(fd.fd) == false)
				{
					toSend(fd.fd, ":ft_irc.com 482 " + clients[fd.fd].nickname + " " + channels[i].getName() + " :You're not a channel operator\r\n");
					return ;
				}
				char isValid = isAValidMode(parse.getArgs().at(1));
				switch (isValid)
				{
					case EXIT_SUCCESS:
					{
						std::string sorted = sortModes(parse.getArgs().at(1), fd.fd, channels[i]);
						if (sorted != "" && checkNumberOfParams(sorted, parse, fd, clients) == EXIT_SUCCESS)
							if (checkModeArgs(sorted, parse, fd, clients, channels[i]) == EXIT_SUCCESS)
								executeModes(sorted, parse, socket, fd, clients, channels[i]);
						break;
					}
					case SIGN_MISSING:
						toSend(fd.fd, ":ft_irc.com 472 " + clients[fd.fd].nickname + " :error with signs\r\n");
						break;
					default:
						toSend(fd.fd, ":ft_irc.com 472 " + clients[fd.fd].nickname + " " + isValid + " :is unknown mode char to me\r\n");
				}
			}
		}
	}
}
