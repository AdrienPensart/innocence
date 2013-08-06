#include "LogObserver.hpp"
#include <Innocence.hpp>
#include <iostream>

namespace Common
{
	LogToNetwork::LogToNetwork(const Network::Host& _debug_server, const Network::Port& _debug_port)
    {
        socket_udp.setDestInfo(_debug_server, _debug_port);
    }

    void LogToNetwork::update(const Message& message)
    {
        socket_udp << message.build();
    }
    
	LogToCollector::LogToCollector() : 
		LogToNetwork(Innocence::LOG_COLLECTOR_IP, Innocence::LOG_COLLECTOR_PORT)
    {
    }

    LogToFile::LogToFile(const std::string& _filepath)
    :filepath(_filepath)
    {
        file.open(filepath.c_str());
        if(!file.is_open())
        {
            throw Common::Exception("Can't open log file");
        }
    }

    void LogToFile::update(const Message& message)
    {
        file << message.build();
    }
    
    LogToConsole::LogToConsole(const std::string& _title)
    :title(_title)
    {
    }

    void LogToConsole::update(const Message& message)
    {
		std::cout << message.build();
    }

} // Common