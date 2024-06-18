/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 11:31:56 by lribette          #+#    #+#             */
/*   Updated: 2024/06/18 18:46:57 by lribette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

void	pass(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels)
{
	int error = 0;

	if (parse.getArgs().size() < 1)
	{
		toSend(fd.fd, ":ft_irc.com 461 " + clients[fd.fd].nickname + " PASS :Not enough parameters\r\n");
		error = 1;
	}
	else
	{
		std::string password = parse.getAllArgs(0);
		if (clients[fd.fd].has_given_a_password == 1)
			toSend(fd.fd, ":ft_irc.com 462 " + clients[fd.fd].nickname + " PASS :You may not reregister\r\n");
		else if (password != socket.getPassword())
		{
			toSend(fd.fd, ":ft_irc.com 464 " + clients[fd.fd].nickname + " PASS :Password incorrect\r\n");
			error = 1;
		}
	}
	if (error == 1)
		socket.ft_erase(fd, channels, "");
	else
		clients[fd.fd].has_given_a_password = 1;
}

void	nick(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels)
{
	(void)socket;
	if (parse.getArgs().size() == 0)
		toSend(fd.fd, ":ft_irc.com 431 NICK " + clients[fd.fd].nickname + " :No nickname given\r\n");
	else if (parse.getArgs().size() > 1 || !isACorrectNickname(parse.getArgs().at(0)))
		toSend(fd.fd, ":ft_irc.com 432 NICK " + parse.getArgs().at(0) + " :Erroneus nickname\r\n");
	else
	{
		for (std::map<int, infoClient>::iterator it = clients.begin(); it != clients.end(); ++it)
		{
			if (it->second.nickname == parse.getArgs().at(0))
			{
				toSend(fd.fd, ":ft_irc.com 433 " + it->second.nickname + " " + parse.getArgs().at(0) + " :Nickname is already in use\r\n");
				return ;
			}
		}
		for (size_t i = 0; i < channels.size(); i++)
			if (channels[i].clientIsInChannel(clients, clients[fd.fd].nickname) == true)
				channels[i].sendToTheChannel(fd.fd, 0, ":" + clients[fd.fd].nickname + " NICK " + parse.getArgs().at(0) + "\r\n");
		toSend(fd.fd, ":" + clients[fd.fd].nickname + " NICK " + parse.getArgs().at(0) + "\r\n");
		clients[fd.fd].nickname = parse.getArgs().at(0);
		clients[fd.fd].has_a_good_nickname = 1;
	}
}

void	user(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels)
{
	(void)channels;
	if (clients[fd.fd].has_a_good_username == 1)
		toSend(fd.fd, ":ft_irc.com 462 " + clients[fd.fd].nickname + " USER :You may not reregister\r\n");
	else if (parse.getArgs().size() != 4)
	{
		toSend(fd.fd, ":ft_irc.com 461 " + clients[fd.fd].nickname + " USER :Not enough parameters\r\n(Parameters: <username> <hostname> <servername> <realname>)\r\n");
		socket.ft_erase(fd, channels, "");
	}
	else
	{
		clients[fd.fd].username = parse.getArgs().at(0);
		clients[fd.fd].hostname = parse.getArgs().at(1);
		clients[fd.fd].servername = parse.getArgs().at(2);
		clients[fd.fd].realname = parse.getAllArgs(3);
		clients[fd.fd].has_a_good_username = 1;
	}
}

void	quit(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels)
{
	(void)socket;
	(void)clients;
	(void)channels;
	std::string reason = "";
	if (parse.getArgs().size() > 0)
		reason = parse.getAllArgs(0);
	socket.ft_erase(fd, channels, reason);
}

void	ping(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels)
{
	(void)socket;
	(void)channels;
	
	if (parse.getArgs().size() != 1)
		toSend(fd.fd, ":ft_irc.com 461 " + clients[fd.fd].nickname + " PONG :Not enough parameters\r\n");
	else if (parse.getArgs().at(0).empty())
		toSend(fd.fd, ":ft_irc.com 409 " + clients[fd.fd].nickname + " :No origin specified\r\n");
	else if (parse.getArgs().at(0) != "ft_irc.com")
		toSend(fd.fd, ":ft_irc.com 402 " + clients[fd.fd].nickname + " " + parse.getArgs().at(0) + " :No such server\r\n");
	else
		toSend(fd.fd, ":ft_irc.com PONG :ft_irc.com\r\n");
}

void	join(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels)
{
	(void)socket;
	std::string 						channelName = parse.getArgs().at(0);
	std::string 						key = parse.getArgs().size() == 2 ? parse.getArgs().at(1) : "";
	std::stringstream					nameStream(channelName);
	std::stringstream					keyStream(key);
	std::map<std::string, std::string>	ck;

	if (parse.getArgs().size() == 0 || parse.getArgs().size() > 2)
	{
		toSend(fd.fd, ":ft_irc.com 461 " + clients[fd.fd].nickname + " JOIN :Wrong number of parameters\r\n");
		return ;
	}
	while (std::getline(nameStream, channelName, ','))
	{
		if (std::getline(keyStream, key, ','))
			ck[channelName] = key;
		else
			ck[channelName] = "";
	}
	for (std::map<std::string, std::string>::iterator it = ck.begin(); it != ck.end(); ++it)
	{
		std::cout << "Channel: " << it->first << " Key: " << it->second << std::endl;
		int	i = channelExists(channels, it->first);
		if (i != -1)
		{
			std::cout << "Channel exists" << std::endl;
			// if key is correct
			if (channels[i].getKey() == it->second || channels[i].getKey() == "")
			{
				std::cout << "Key is correct" << std::endl;
				// if client is already in the channel
				if (channels[i].getClients().find(fd.fd) != channels[i].getClients().end())
					toSend(fd.fd, "You are already in the channel\r\n");
				// if client is not in the channel
				else if (checkInvitesAndLimit(fd, clients, channels[i], it->first) == EXIT_SUCCESS)
				{
					std::cout << "You have joined the channel" << std::endl;
					// add client to the channel
					channels[i].push(fd.fd, "");
					std::string	listOfUsers;
					for (std::map<int, std::string>::iterator it = channels[i].getClients().begin(); it != channels[i].getClients().end();)
					{
						listOfUsers += it->second + clients.find(it->first)->second.nickname;
						if (it->first != fd.fd)
							toSend(it->first, getCompleteName(fd, clients) + " JOIN " + channels[i].getName() + "\r\n");
						++it;
						if (it != channels[i].getClients().end())
							listOfUsers += " ";
					}
					toSend(fd.fd, getCompleteName(fd, clients) + " JOIN " + channels[i].getName() + "\r\n");
					// send RPL_TOPIC
					toSend(fd.fd, ":ft_irc.com 332 " + clients[fd.fd].nickname + " " + channels[i].getName() + " :" + channels[i].getTopic() + "\r\n");
					// send list of users in the channel (RPL_NAMREPLY)
					toSend(fd.fd, ":ft_irc.com 353 " + clients[fd.fd].nickname + " = " + channels[i].getName() + " :" + listOfUsers + "\r\n");
					toSend(fd.fd, ":ft_irc.com 366 " + clients[fd.fd].nickname + " " + channels[i].getName() + " :" + "End of /NAMES list.\r\n");
				}
			}
			// if key is wrong
			else
			{
				std::cout << "Invalid key" << std::endl;
				toSend(fd.fd, ":ft_irc.com 475 " + clients[fd.fd].nickname + " " + channels[i].getName() + " :" + "Cannot join channel (+k)\r\n");
			}
		}
		else
		{
			std::cout << "Channel does not exist" << std::endl;
			// create a new channel
			Channel	c(fd.fd, it->first, it->second, "No topic is set.", "@");
			// add channel to the list of channels
			channels.push_back(c.getChannel());
			// send RPL_TOPIC
			toSend(fd.fd, getCompleteName(fd, clients) + " JOIN " + c.getName() + "\r\n");
			toSend(fd.fd, ":ft_irc.com 332 " + clients[fd.fd].nickname + " " + c.getName() + " :" + c.getTopic() + "\r\n");
			// send list of users in the channel (RPL_NAMREPLY)
			toSend(fd.fd, ":ft_irc.com 353 " + clients[fd.fd].nickname + " = " + c.getName() + " :@" + clients[fd.fd].nickname + "\r\n");
			toSend(fd.fd, ":ft_irc.com 366 " + clients[fd.fd].nickname + " " + c.getName() + " :" + "End of /NAMES list.\r\n");
		}
	}
}

void	part(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels)
{
	(void)socket;

	if (parse.getArgs().size() == 0)
		toSend(fd.fd, ":ft_irc.com 461 " + clients[fd.fd].nickname + " PART :Not enough parameters\r\n");
	else
	{
		std::string					channelName = parse.getArgs().at(0);
		std::stringstream			nameStream(channelName);
		std::vector<std::string>	channelNames;

		while (std::getline(nameStream, channelName, ','))
			channelNames.push_back(channelName);
		for (unsigned long int i = 0; i < channelNames.size(); i++)
		{
			int	j = channelExists(channels, channelNames[i]);
			if (j == -1)
				toSend(fd.fd, ":ft_irc.com 403 " + clients[fd.fd].nickname + " " + channelNames[i] + " :No such channel\r\n");
			else
			{
				if (!channels[j].clientIsInChannel(fd.fd))
                    toSend(fd.fd, ":ft_irc.com 442 " + clients[fd.fd].nickname + " " + channelNames[i] + " :You're not on that channel\r\n");
                else
                {
                    std::string reason = parse.getArgs().size() == 2 ? parse.getArgs().at(1) : "Goodbye";
					channels[i].sendToTheChannel(fd.fd, 1, getCompleteName(fd, clients) + " PART " + channelNames[i] + " :" + reason + "\r\n");
                    channels[j].pop(fd.fd);
                    if (channels[j].isEmpty())
                        channels.erase(channels.begin() + j);
				}
			}
		}
	}
}

void	topic(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels)
{
	(void)socket;

	if (parse.getArgs().size() == 0)
		toSend(fd.fd, ":ft_irc.com 461 " + clients[fd.fd].nickname + " TOPIC :Not enough parameters\r\n");
	else
	{
		int	i = channelExists(channels, parse.getArgs().at(0));
		if (i == -1)
			toSend(fd.fd, ":ft_irc.com 403 " + clients[fd.fd].nickname + " " + parse.getArgs().at(0) + " :No such channel\r\n");
		else
		{
			if (parse.getArgs().size() == 1 && channels[i].getTopic() == "No topic is set.")
				toSend(fd.fd, ":ft_irc.com 331 " + clients[fd.fd].nickname + " " + parse.getArgs().at(0) + " :No topic is set.\r\n");
			else if (parse.getArgs().size() > 1)
			{
				if (!channels[i].clientIsInChannel(fd.fd))
					toSend(fd.fd, ":ft_irc.com 442 " + clients[fd.fd].nickname + " " + parse.getArgs().at(0) + " :You're not on that channel\r\n");
				else
				{
					if (channels[i].clientIsOperator(fd.fd) == true)
					{
						channels[i].setTopic(parse.getAllArgs(1));
						channels[i].sendToTheChannel(fd.fd, 1, getCompleteName(fd, clients) + " TOPIC " + channels[i].getName() + " :" + channels[i].getTopic() + "\r\n");
					}
					else
					{
						if (channels[i].getMode('t', fd.fd) == "+t")
						{
							channels[i].setTopic(parse.getAllArgs(1));
							channels[i].sendToTheChannel(fd.fd, 1, getCompleteName(fd, clients) + " TOPIC " + channels[i].getName() + " :" + channels[i].getTopic() + "\r\n");
						}
						else
							toSend(fd.fd, ":ft_irc.com 482 " + clients[fd.fd].nickname + " " + parse.getArgs().at(0) + " :You're not a channel operator\r\n");
					}
				}
			}
		}
	}
}

void	privmsg(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels)
{
	// :Alice!alice@192.168.1.10 PRIVMSG #example :Bonjour tout le monde !
	if (parse.getArgs().size() <= 1)
		toSend(fd.fd, ":ft_irc.com 461 " + clients[fd.fd].nickname + " TOPIC :Not enough parameters\r\n");
	else
	{
		std::string			targetName = parse.getArgs().at(0);
		std::stringstream	targetStream(targetName);

		while (std::getline(targetStream, targetName, ','))
		{
			if (targetName[0] == '#')
			{
				std::cout << "This is a channel" << std::endl;
				int	i = channelExists(channels, targetName);
				if (i == -1)
					toSend(fd.fd, ":ft_irc.com 403 " + clients[fd.fd].nickname + " " + targetName + " :No such channel\r\n");
				else
				{
					std::cout << "Channel exists" << std::endl;
					if (!channels[i].clientIsInChannel(fd.fd))
						toSend(fd.fd, ":ft_irc.com 442 " + clients[fd.fd].nickname + " " + targetName + " :You're not on that channel\r\n");
					else
					{
						std::string	message = parse.getAllArgs(1);
						channels[i].sendToTheChannel(fd.fd, 0, getCompleteName(fd, clients) + " PRIVMSG " + channels[i].getName() + " :" + message + "\r\n");
					}
				}
			}
			else
			{
				std::cout << "This is a user" << std::endl;
				if (!nicknameExists(clients, targetName))
					toSend(fd.fd, ":ft_irc.com 401 " + clients[fd.fd].nickname + " " + targetName + " :No such nick\r\n");
				else
				{
					std::string	message = parse.getAllArgs(1);
					toSend(socket.getClientFd(targetName), getCompleteName(fd, clients) + " PRIVMSG " + targetName + " :" + message + "\r\n");
				}
			}
		}
	}
}

void	invite(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels)
{
	if (parse.getArgs().size() != 2)
		toSend(fd.fd, ":ft_irc.com 461 " + clients[fd.fd].nickname + " INVITE :Not enough parameters\r\n");
	else
	{
		std::string		nickname = parse.getArgs().at(0);
		std::string		channelName = parse.getArgs().at(1);
		int				i = channelExists(channels, channelName);
		if (i == -1)
			toSend(fd.fd, ":ft_irc.com 403 " + clients[fd.fd].nickname + " " + channelName + " :No such channel\r\n");
		else
		{
			if (!channels[i].clientIsInChannel(fd.fd))
				toSend(fd.fd, ":ft_irc.com 442 " + clients[fd.fd].nickname + " " + channelName + " :You're not on that channel\r\n");
			else if (!nicknameExists(clients, nickname))
				toSend(fd.fd, ":ft_irc.com 401 " + clients[fd.fd].nickname + " " + nickname + " :No such nick\r\n");
			else if (channels[i].getMode('i', fd.fd) == "+i" && !channels[i].clientIsOperator(fd.fd))
				toSend(fd.fd, ":ft_irc.com 482 " + clients[fd.fd].nickname + " " + channelName + " :You're not a channel operator\r\n");
			else if (channels[i].clientIsInChannel(clients, nickname))
				toSend(fd.fd, ":ft_irc.com 443 " + clients[fd.fd].nickname + " " + nickname + " " + channelName + " :is already on channel\r\n");
			else
			{
				toSend(fd.fd, ":ft_irc.com 341 " + clients[fd.fd].nickname + " " + nickname + " " + channelName + "\r\n");
				toSend(socket.getClientFd(nickname), getCompleteName(fd, clients) + " INVITE " + nickname + " " + channelName + "\r\n");
				for (size_t i = 0; i < clients[socket.getClientFd(nickname)].channelsInvitingMe.size(); i++)
					if (clients[socket.getClientFd(nickname)].channelsInvitingMe.at(i) == channelName)
						return ;
				clients[socket.getClientFd(nickname)].channelsInvitingMe.push_back(channelName);
			}
		}
	}
}

void	kick(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels)
{
	if (parse.getArgs().size() < 2)
		toSend(fd.fd, ":ft_irc.com 461 " + clients[fd.fd].nickname + " KICK :Not enough parameters\r\n");
	else
	{
		std::string	channelName = parse.getArgs().at(0);
		int	i = channelExists(channels, channelName);
		if (i == -1)
			toSend(fd.fd, ":ft_irc.com 403 " + clients[fd.fd].nickname + " " + channelName + " :No such channel\r\n");
		else
		{
			if (!channels[i].clientIsInChannel(fd.fd))
				toSend(fd.fd, ":ft_irc.com 442 " + clients[fd.fd].nickname + " " + channelName + " :You're not on that channel\r\n");
			else if (!channels[i].clientIsOperator(fd.fd))
				toSend(fd.fd, ":ft_irc.com 482 " + clients[fd.fd].nickname + " " + channelName + " :You're not a channel operator\r\n");
			else
			{
				std::string			targetName = parse.getArgs().at(1);
				std::stringstream	targetStream(targetName);
				while (std::getline(targetStream, targetName, ','))
				{
					if (!channels[i].clientIsInChannel(clients, targetName))
						toSend(fd.fd, ":ft_irc.com 401 " + clients[fd.fd].nickname + " " + targetName + " :No such nick\r\n");
					else
					{
						std::string	reason = "";
						if (parse.getArgs().size() == 2 || (parse.getArgs().size() == 3 && parse.getArgs().at(2) == ""))
							reason = "Goodbye!";
						else
							reason = parse.getAllArgs(2);
						channels[i].sendToTheChannel(fd.fd, 1, ":ft_irc.com KICK " + channels[i].getName() + " " + targetName + " :" + reason + "\r\n");
						channels[i].pop(socket.getClientFd(targetName));
						if (channels[i].isEmpty())
							channels.erase(channels.begin() + i);
					}
				}
			}
		}
	}
}

void    which_command(Parse& parse, Socket& socket, struct pollfd& fd, std::map<int, infoClient>& clients, std::vector<class Channel>& channels)
{
	size_t		i = 0;
	std::string	cmdptr[] = {"PASS", "NICK", "USER", "QUIT", "PING", "JOIN", "PART", "TOPIC", "PRIVMSG", "MODE", "INVITE", "KICK"};
	void		(*fxptr[])(Parse&, Socket&, struct pollfd&, std::map<int, infoClient>&, std::vector<class Channel>&) = {pass, nick, user, quit, ping, join, part, topic, privmsg, mode, invite, kick};

	while (parse.getCmd() != cmdptr[i])
	{
		i++;
		if (i > 11)
			return ;
		// NE PAS OUBLIER DE REMETTRE EN PLACE CORRECTEMENT ICI
	}
	if (isRegistered(parse, socket, fd, clients, channels, cmdptr[i]))
		(*fxptr[i])(parse, socket, fd, clients, channels);
}
