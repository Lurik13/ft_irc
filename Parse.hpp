#pragma once

#include "server.hpp"

class Parse
{
    private:
        std::string                 _server;
        std::string                 _nickname;
        std::string                 _username;
        std::string                 _hostname;
        std::string                 _prefix;
        std::string                 _cmd;
        std::vector<std::string>    _args;
    public:
        Parse();
        ~Parse();
        void                        parse(std::string msg);
};
