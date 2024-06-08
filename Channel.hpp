/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 10:45:05 by lribette          #+#    #+#             */
/*   Updated: 2024/06/08 12:06:56 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Socket.hpp"
#include "server.hpp"

struct infoClient;

class Channel
{
	private:
		std::map<int, std::string>	_clients;
		std::string					_name;
		std::string                 _key;
		std::string                 _topic; // topic du channel (EST-CE QUE C'EST UNE STRING LE TOPIC?)
	public:
		Channel(void);
		~Channel(void);
		void						push(std::map<int, std::string>::iterator it, std::string& name, std::string key, std::string topic);
		std::map<int, std::string>&	getClients(void);
		std::string					getName(void);
		std::string                 getKey(void);
		std::string                 getTopic(void);
		Channel&					getChannel(void);
};
