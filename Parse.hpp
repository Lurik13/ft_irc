/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 21:17:53 by lribette          #+#    #+#             */
/*   Updated: 2024/06/18 17:03:33 by lribette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "server.hpp"

class Parse
{
	private:
		std::string					_server;
		std::string					_nickname;
		std::string					_username;
		std::string					_hostname;
		std::string					_prefix;
		std::string					_cmd;
		std::vector<std::string>	_args;
		std::string					pcharToString(char *pchar);
        void						prefix(void);
	public:
		Parse();
		~Parse();
		std::string					getServer(void);
		std::string					getNickname(void);
		std::string					getUsername(void);
		std::string					getHostname(void);
		std::string					getCmd(void);
		std::vector<std::string>	getArgs(void);
		std::string 				getAllArgs(int begin);
		Parse&						parse(std::string msg);
};

std::string	substr_to_limiter(int *start, std::string prefix, char limiter);