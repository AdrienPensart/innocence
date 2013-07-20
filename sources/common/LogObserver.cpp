#include "LogObserver.hpp"

#include <iostream>

namespace Common
{
	LogToNetwork::LogToNetwork(const Network::Host& _debug_server, const Network::Port& _debug_port)
    {
        socket_udp.setDestInfo(_debug_server, _debug_port);
    }

    void LogToNetwork::update(const std::string& object)
    {
        socket_udp << object;
    }
    
    LogToFile::LogToFile(const std::string& _filepath)
    :filepath(_filepath)
    {
        file.open(filepath.c_str());
        if(!file.is_open())
        {
            throw std::exception("Can't open log file.");
        }
    }

    void LogToFile::update(const std::string& object)
    {
        file << object;
    }
    
    LogToConsole::LogToConsole(const std::string& _title)
    :title(_title)
    {
    }

    void LogToConsole::update(const std::string& object)
    {
		std::cout << object;
    }

} // Common