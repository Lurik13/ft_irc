/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 10:45:05 by lribette          #+#    #+#             */
/*   Updated: 2024/05/31 11:43:34 by lribette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Socket.hpp"
#include "server.hpp"

struct infoClient;

class Channel
{
    private:
        std::map<int, infoClient>   _clients;
        std::string                 _key;
        std::string                 _topic; // topic du channel (EST-CE QUE C'EST UNE STRING LE TOPIC?)
    public:
        Channel(void);
        ~Channel(void);
        void                        push(std::map<int, infoClient>::iterator it);
        std::map<int, infoClient>   getClients(void);
        std::string                 getKey(void);
        std::string                 getTopic(void);
};
