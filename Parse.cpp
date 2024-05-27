#include "Parse.hpp"

Parse::Parse() : _server(""), _nickname(""), _username(""), _hostname(""), _prefix(""), _cmd(""), _args(std::vector<std::string>()) {}

Parse::~Parse() {}

void    Parse::parse(std::string msg)
{
    std::istringstream  stream(msg);
    std::string         token;

    stream >> std::ws; // skip whitespace characters (spaces, tabs, newlines, etc...) at the beginning of the stream
    if (stream.peek() == ':')
    {
        std::getline(stream, token, ' ');
    }
    std::cout << "|" << token << "|" << std::endl;
}
