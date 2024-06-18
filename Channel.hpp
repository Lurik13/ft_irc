/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 10:45:05 by lribette          #+#    #+#             */
/*   Updated: 2024/06/18 17:08:26 by lribette         ###   ########.fr       */
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
		std::string					_key;
		std::string					_topic;
		int							_nbMaxOfClients;
		bool						_isInviteOnly;
		bool						_canDefineTopic;
	public:
		Channel(void);
		Channel(int fd, std::string name, std::string key, std::string topic, std::string mode);
		~Channel(void);
		void						push(int fd, std::string mode);
		void						sendToTheChannel(int fd, bool sendToMe, std::string str);
		
		std::map<int, std::string>&	getClients(void);
		int							getClientsNumber(void);
		std::string					getName(void);
		std::string					getKey(void);
		std::string					getTopic(void);
		int							getNbMaxOfClients(void);
		bool						getIsInviteOnly(void);
		bool						getCanDefineTopic(void);
		Channel&					getChannel(void);
		std::string					getMode(char mode, int fd);
		
		void						setIsInviteOnly(bool invite);
		void						setCanDefineTopic(bool topic);
		void						setTopic(std::string topic);
		void						setKey(std::string key);
		void						setOperator(int fd, std::string permission);
		void						setLimit(int limit);
		
		void						pop(int fd);
		bool						isEmpty(void);
		
		bool						clientIsInChannel(int fd);
		bool						clientIsInChannel(const std::map<int, infoClient>& clients, std::string nickname);
		bool						clientIsOperator(int fd);
};
