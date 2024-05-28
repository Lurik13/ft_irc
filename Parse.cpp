#include "Parse.hpp"

Parse::Parse() : _server(""), _nickname(""), _username(""), _hostname(""), _prefix(""), _cmd(""), _args(std::vector<std::string>()) {}

Parse::~Parse() {}

std::string Parse::pcharToString(char *pchar)
{
	std::string str = pchar;
	return (str);
}

void    Parse::parse(std::string msg)
{
	std::istringstream  stream(msg);
	std::string         token;

	// PREFIX
	stream >> std::ws;
	if (stream.peek() == ':')
	{
		stream >> token;
		this->_prefix = token.substr(1, token.size() - 1);
	}

	// CMD
	stream >> token;
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

    this->prefix();
}

void	Parse::prefix(void)
{
    if (this->_prefix.empty())
        return ;
	// int start = 0;
	// for (int i = 0; this->_prefix[i]; i++)
	// {
	// 	if ()
	// }
}

// :nickname!username@hostname