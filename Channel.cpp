/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 10:45:09 by lribette          #+#    #+#             */
/*   Updated: 2024/06/13 17:40:58 by lribette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(void)
{
}

Channel::Channel(int fd, std::string name, std::string key, std::string topic, std::string mode)
{
	_clients.clear();
	_clients[fd].name = mode;
	_name = name;
	_key = key;
	_topic = topic;
	_nbMaxOfClients = 2147483647;
	_isInviteOnly = 0;
	_clients[fd].canDefineTopic = 1;
	_clients[fd].canGiveOp = 1;
}

Channel::~Channel(void)
{
}

void    Channel::push(int fd, std::string mode)
{
	_clients[fd].name = mode;
	_clients[fd].canDefineTopic = 0;
	_clients[fd].canGiveOp = 0;
}

std::map<int, clientData>&	Channel::getClients(void)
{
	return (this->_clients);
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

bool	Channel::getCanDefineTopic(int fd)
{
	return (this->_clients[fd].canDefineTopic);
}

bool	Channel::getCanGiveOp(int fd)
{
	return (this->_clients[fd].canGiveOp);
}

Channel&	Channel::getChannel(void)
{
	return (*this);
}

void	Channel::setTopic(std::string topic)
{
	this->_topic = topic;
}

void	Channel::setCanGiveOp(int fd, bool op)
{
	this->_clients[fd].canGiveOp = op;
}

bool	Channel::clientIsInChannel(int fd)
{
	if (_clients.find(fd) == _clients.end())
		return (false);
	return (true);
}

bool	Channel::clientIsInChannel(const std::map<int, infoClient>& clients, std::string nickname)
{
	for (std::map<int, clientData>::iterator it = _clients.begin(); it != _clients.end(); it++)
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
