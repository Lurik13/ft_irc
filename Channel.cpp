/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 10:45:09 by lribette          #+#    #+#             */
/*   Updated: 2024/06/03 21:54:16 by lribette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(void)
{
}

Channel&	Channel::Channel(int fd, std::string name, std::string key, std::string topic, std::string mode)
{
	std::map<int, std::string>::iterator it;

	it = _clients.begin();
	it->first = fd;
	it->second = mode;
	push(it, name, key, topic);
	return (*this);
}

Channel::~Channel(void)
{
}

void    Channel::push(std::map<int, std::string>::iterator it, std::string& name, std::string key, std::string topic)
{
	_clients.insert(*it);
	_name = name;
	_key = key;
	_topic = topic;
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
