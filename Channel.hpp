/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 10:45:05 by lribette          #+#    #+#             */
/*   Updated: 2024/06/07 23:17:17 by lribette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Socket.hpp"
#include "server.hpp"

#define OPERATOR 3
#define IN_CHANNEL 2
#define NOT_IN_CHANNEL 1

struct infoClient;

class Channel
{
	private:
		std::map<int, infoClient>   _clients;
		// std::map<int, int> _clients;
		std::string					_name;
		std::string                 _key;
		std::string                 _topic; // topic du channel (EST-CE QUE C'EST UNE STRING LE TOPIC?)
	public:
		Channel(void);
		~Channel(void);
		void						push(std::map<int, infoClient>::iterator it, std::string& name, std::string key, std::string topic);
		std::map<int, infoClient>&	getClients(void);
		std::string					getName(void);
		std::string                 getKey(void);
		std::string                 getTopic(void);
		Channel&					getChannel(void);
};
