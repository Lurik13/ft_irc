/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 10:42:25 by lribette          #+#    #+#             */
/*   Updated: 2024/06/16 11:02:51 by lribette         ###   ########.fr       */
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
		{
			std::cout << "i = " << i << " has been removed" << std::endl;
			str.erase(str.begin()+i);
		}
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
	std::cout << CYAN << str << RESET << std::endl;
	str = removeSigns(str);
	return (str);
}

bool	checkNumberOfParams(std::string str, Parse& parse, struct pollfd& fd, std::map<int, infoClient>& clients)
{
	size_t nbOfParamsNeeded = 0;
	for (int i = 1; str[i]; i++)
	{
		if (str[i] == 'o')
			nbOfParamsNeeded++;
		else if (str[i] == 'k' || str[i] == 'l')
		{
			int j = i - 1;
			while (str[j] != '+' && str[j] != '-')
				j--;
			if (str[j] == '+')
				nbOfParamsNeeded++;
		}
	}
	std::cout << YELLOW << "nbOfParamsNeeded = " << nbOfParamsNeeded << RESET << std::endl;
	if (nbOfParamsNeeded != parse.getArgs().size() - 2)
	{
		toSend(fd.fd, ":ft_irc.com 461 " + clients[fd.fd].nickname + " MODE :Not enough parameters\r\n");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
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

bool	checkModeArgs(std::string str, Parse& parse, struct pollfd& fd, std::map<int, infoClient>& clients, Channel& channel)
{
	int argIndex = 2;
	for (int i = 1; str[i]; i++)
	{
		if (str[i] == 'k' && whichSign(str, i) == "+k")
			argIndex++;
		else if (str[i] == 'o')
		{
			if (channel.clientIsInChannel(clients, parse.getArgs().at(argIndex)) == 0)
			{
				toSend(fd.fd, ":ft_irc.com 401 " + clients[fd.fd].nickname + " " + parse.getArgs().at(argIndex) + " :No such nick.\r\n");
				return (EXIT_FAILURE);
			}
			argIndex++;
		// bool						clientIsOperator(int fd, std::map<int, infoClient>& clients);
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
			// channel.setLimit(std::atoi(parse.getArgs().at(argIndex).c_str()));
		}
	}
	return (EXIT_SUCCESS);
}

void	executeModes(std::string str, Parse& parse, struct pollfd& fd, std::map<int, infoClient>& clients, Channel& channel)
{
	(void)str;
	(void)parse;
	(void)fd;
	(void)clients;
	(void)channel;

	// for (int i = 1; str[i]; i++)
	// {
		
	// }
	// NE PAS OUBLIER DE PRECISER A CHAQUE CLIENT QU'ON A MODIFIE LES PERMISSIONS
}

// — i : Définir/supprimer le canal sur invitation uniquement
// — t : Définir/supprimer les restrictions de la commande TOPIC pour les opérateurs de canaux
// — k : Définir/supprimer la clé du canal (mot de passe)
// — o : Donner/retirer le privilège de l’opérateur de canal
// — l : Définir/supprimer la limite d’utilisateurs pour le canal
void	mode(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels)
{
	(void)socket;
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
				toSend(fd.fd, ":" + clients[fd.fd].nickname + "!" + clients[fd.fd].username + "@" + clients[fd.fd].hostname + " MODE " + channels[i].getName() + allModes + "\r\n");
			}
			else if (parse.getArgs().size() > 1)
			{
				char isValid = isAValidMode(parse.getArgs().at(1));
				switch (isValid)
				{
					case EXIT_SUCCESS:
					{
						std::string sorted = sortModes(parse.getArgs().at(1), fd.fd, channels[i]);
						std::cout << RED << "signSorted = " << sorted << RESET << std::endl;
						if (sorted != "" && checkNumberOfParams(sorted, parse, fd, clients) == EXIT_SUCCESS)
							if (checkModeArgs(sorted, parse, fd, clients, channels[i]) == EXIT_SUCCESS)
								executeModes(sorted, parse, fd, clients, channels[i]);
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

// +ii-i+i-i+o-kl+io