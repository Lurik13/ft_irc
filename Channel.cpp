/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 10:45:09 by lribette          #+#    #+#             */
/*   Updated: 2024/05/31 10:45:10 by lribette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(void)
{
}

Channel::~Channel(void)
{
}

void    Channel::push(std::map<int, infoClient>::iterator it)
{
    _clients.insert(*it);
}


std::map<int, infoClient>   Channel::getClients(void)
{
    return (this->_clients);
}

std::string Channel::getKey(void)
{
    return (this->_key);
}

std::string Channel::getTopic(void)
{
    return (this->_topic);
}
