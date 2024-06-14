/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 10:45:05 by lribette          #+#    #+#             */
/*   Updated: 2024/06/13 17:41:02 by lribette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Socket.hpp"
#include "server.hpp"

struct infoClient;

typedef struct clientData
{
	std::string	name;
	bool		canDefineTopic;
	bool		canGiveOp;
} clientData;

class Channel
{
	private:
		std::map<int, clientData>	_clients;
		std::string					_name;
		std::string                 _key;
		std::string                 _topic;
		int							_nbMaxOfClients;
		bool						_isInviteOnly;
	public:
		Channel(void);
		Channel(int fd, std::string name, std::string key, std::string topic, std::string mode);
		~Channel(void);
		void						push(int fd, std::string mode);
		std::map<int, clientData>&	getClients(void);
		std::string					getName(void);
		std::string                 getKey(void);
		std::string                 getTopic(void);
		int							getNbMaxOfClients(void);
		bool						getIsInviteOnly(void);
		bool						getCanDefineTopic(int fd);
		bool						getCanGiveOp(int fd);
		Channel&					getChannel(void);
		void						setTopic(std::string topic);
		void						setCanGiveOp(int fd, bool op);
		void						pop(int fd);
		bool						isEmpty(void);
		bool						clientIsInChannel(int fd);
		bool						clientIsInChannel(const std::map<int, infoClient>& clients, std::string nickname);
};
