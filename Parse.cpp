/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 21:17:04 by lribette          #+#    #+#             */
/*   Updated: 2024/05/28 21:17:04 by lribette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parse.hpp"

Parse::Parse() : _server(""), _nickname(""), _username(""), _hostname(""), _prefix(""), _cmd("")
{
	_args.clear();
}

Parse::~Parse() {}

std::string	Parse::getServer(void) {return this->_server;}
std::string	Parse::getNickname(void) {return this->_nickname;}
std::string	Parse::getUsername(void) {return this->_username;}
std::string	Parse::getHostname(void) {return this->_hostname;}

std::string Parse::pcharToString(char *pchar)
{
	std::string str = pchar;
	return (str);
}

void    Parse::parse(std::string msg)
{
	std::istringstream  stream(msg);
	std::string         token;

	// REINITIALISER LES ATTRIBUTS A 0 ENTRE CHAQUE APPEL DE LA FONCTION
	this->_server = "";
	this->_nickname = "";
	this->_username = "";
	this->_hostname = "";
	this->_prefix = "";
	this->_cmd = "";
	this->_args.clear();

	// PREFIX
	stream >> std::ws;
	if (stream.peek() == ':')
	{
		stream >> token;
		this->_prefix = token.substr(1, token.size() - 1);
	}

	// CMD
	if (stream >> token)
		this->_cmd = token;

	// ARGS
	while (stream >> token)
	{
		// If an argument starts with ':', it includes all the rest of the message
		if (token[0] == ':')
		{
			std::string buffer;
			std::getline(stream, buffer);
			token = token.substr(1) + buffer; // Remove the leading ':'
			this->_args.push_back(token);
			break;
		}
		else
			this->_args.push_back(token);
	}

	std::cout << "cmd = " << this->_cmd << std::endl;
	std::cout << "args = " << std::endl;
	if (!this->_args.empty())
		for (unsigned long i = 0; i < this->_args.size(); i++)
			std::cout << " - " << this->_args.at(i) << std::endl;
	std::cout << "prefix = " << this->_prefix << std::endl;
    this->prefix();
}

std::string	substr_to_limiter(int *start, std::string prefix, char limiter)
{
	int begin = *start;
	int i = begin;
	while (prefix[i])
	{
		if (prefix[i] == limiter)
			break ;
		i++;
	}
	*start = i + 1;
	return (prefix.std::string::substr(begin, i - begin));
}

void	Parse::prefix(void)
{
    if (this->_prefix.empty())
	{
        return ;
	}
	int start = 0;
	this->_nickname = substr_to_limiter(&start, this->_prefix, '!');
	this->_username = substr_to_limiter(&start, this->_prefix, '@');
	this->_hostname = substr_to_limiter(&start, this->_prefix, ' ');
	std::cout
	<< "nickname = " << this->_nickname << std::endl
	<< "username = " << this->_username << std::endl
	<< "hostname = " << this->_hostname << std::endl;
}

// :nickname!username@hostname CMD arg1 arg2 arg3 :arg4 arg4 arg4