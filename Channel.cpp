/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 10:45:09 by lribette          #+#    #+#             */
/*   Updated: 2024/06/11 12:01:37 by lribette         ###   ########.fr       */
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
}

Channel::~Channel(void)
{
}

void    Channel::push(int fd, std::string mode)
{
	_clients[fd] = mode;
}

std::map<int, std::string>&	Channel::getClients(void)
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

Channel&	Channel::getChannel(void)
{
	return (*this);
}

void	Channel::setTopic(std::string topic)
{
	this->_topic = topic;
}

bool	Channel::clientIsInChannel(int fd)
{
	if (_clients.find(fd) == _clients.end())
		return (false);
	return (true);
}

void	Channel::pop(int fd)
{
	_clients.erase(fd);
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
