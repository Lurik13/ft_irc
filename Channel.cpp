/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 10:45:09 by lribette          #+#    #+#             */
/*   Updated: 2024/06/18 17:08:50 by lribette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(void)
{
}

Channel::Channel(int fd, std::string name, std::string key, std::string topic, std::string mode)
{
	_clients.clear();
	_clients[fd] = mode;
	_name = name;
	_key = key;
	_topic = topic;
	_nbMaxOfClients = 2147483647;
	_isInviteOnly = 0;
	_canDefineTopic = 1;
}

Channel::~Channel(void)
{
}

void    Channel::push(int fd, std::string mode)
{
	_clients[fd] = mode;
}

void	Channel::sendToTheChannel(int fd, bool sendToMe, std::string str)
{
	if (sendToMe == 1)
	{
		std::cout << CYAN << fd << " sent " << str << RESET << std::endl;
		send(fd, str.c_str(), str.size(), MSG_NOSIGNAL);
	}
	for (std::map<int, std::string>::iterator it = this->getClients().begin(); it != this->getClients().end();)
	{
		if (it->first != fd)
		{
			std::cout << GREEN << fd << " sent to " << it->first << " " << str << RESET << std::endl;
			send(it->first, str.c_str(), str.size(), MSG_NOSIGNAL);
		}
		++it;
	}
}

std::map<int, std::string>&	Channel::getClients(void)
{
	return (this->_clients);
}

int	Channel::getClientsNumber(void)
{
	return (this->_clients.size());
}

std::string Channel::getName(void)
{
	return (this->_name);
}

std::string Channel::getKey(void)
{
	return (this->_key);
}

std::string Channel::getTopic(void)
{
	return (this->_topic);
}

int	Channel::getNbMaxOfClients(void)
{
	return (this->_nbMaxOfClients);
}

bool	Channel::getIsInviteOnly(void)
{
	return (this->_isInviteOnly);
}

bool	Channel::getCanDefineTopic(void)
{
	return (this->_canDefineTopic);
}

Channel&	Channel::getChannel(void)
{
	return (*this);
}

std::string	Channel::getMode(char mode, int fd)
{
	std::string result = "";
	if (mode == 'i')
	{
		if (this->getIsInviteOnly() == 0)
			return ("-i");
		return ("+i");
	}
	else if (mode == 't')
	{
		if (this->getCanDefineTopic() == 0)
			return ("-t");
		return ("+t");
	}
	else if (mode == 'k')
	{
		if (this->getKey() == "")
			return ("-k");
		return ("+k");
	}
	else if (mode == 'o')
	{
		if (this->getClients()[fd] == "")
			return ("-o");
		return ("+o");
	}
	else //if (mode == 'l')
	{
		if (this->getNbMaxOfClients() == 2147483647)
			return ("-l");
		return ("+l");
	}
}

void	Channel::setIsInviteOnly(bool invite)
{
	this->_isInviteOnly = invite;
}

void	Channel::setCanDefineTopic(bool topic)
{
	this->_canDefineTopic = topic;
}

void	Channel::setTopic(std::string topic)
{
	this->_topic = topic;
}

void	Channel::setKey(std::string key)
{
	this->_key = key;
}

void	Channel::setOperator(int fd, std::string permission)
{
	this->_clients[fd] = permission;
}

void	Channel::setLimit(int limit)
{
	this->_nbMaxOfClients = limit;
}

bool	Channel::clientIsInChannel(int fd)
{
	if (_clients.find(fd) == _clients.end())
		return (false);
	return (true);
}

bool	Channel::clientIsInChannel(const std::map<int, infoClient>& clients, std::string nickname)
{
	for (std::map<int, std::string>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (clients.find(it->first)->second.nickname == nickname)
			return (true);
	}
	return (false);
}

void	Channel::pop(int fd)
{
	_clients.erase(fd);
}

bool	Channel::isEmpty(void)
{
	if (_clients.empty())
		return (true);
	return (false);
}

bool	Channel::clientIsOperator(int fd)
{
	for (std::map<int, std::string>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (_clients.find(it->first)->first == fd)
		{
			if (_clients[fd] == "@")
				return (true);
			return (false);
		}
	}
	return (false);
}


bool    isChannel(std::string str)
{
	if (str.size() < 2 || str.size() > 50)
		return (0);
	if (str[0] != '#')
		return (0);
	for (int i = 1; str[i]; i++)
		if (str[i] == ' ' || str[i] == '\a' || str[i] == ',' || str[i] == ':')
			return (0);
	return (1);
}
