#include "Parse.hpp"

Parse::Parse() : _server(""), _nickname(""), _username(""), _hostname(""), _prefix(""), _cmd(""), _args(std::vector<std::string>()) {}

Parse::~Parse() {}

void    Parse::parse(std::string msg)
{
    
}
