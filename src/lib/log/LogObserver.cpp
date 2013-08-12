#include "LogObserver.hpp"
#include "Log.hpp"
#include "Message.hpp"

#include <system/File.hpp>
#include <common/Settings.hpp>
#include <iostream>

namespace Log
{
	LogToNetwork::LogToNetwork(const Network::Host& loggerIp, const Network::Port& loggerPort)
    {
        //socket.setDestInfo(loggerIp, loggerPort);
		connected = socket.connect(loggerIp, loggerPort);
		if(!connected)
		{
			LOG << "Failed to connect to network observer on " + loggerIp + ":" + Common::toString(loggerPort);
		}
    }

    void LogToNetwork::update(const Message& message)
    {
		try
		{
			if(connected)
			{
				socket << message.serialize() + '\n';
			}
		}
		catch(Network::Deconnection&)
		{
			connected = false;
		}
    }
    
	Network::Stream&  LogToNetwork::getStream()
	{
		return socket;
	}

	LogToCollector::LogToCollector() : 
		LogToNetwork(Common::LOG_COLLECTOR_IP, Common::LOG_COLLECTOR_PORT)
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
		file << message.getIdentity().getModule() << message.getContent();
    }
    
    LogToConsole::LogToConsole(const std::string& _title)
    :title(_title)
    {
    }

    void LogToConsole::update(const Message& message)
    {
		std::cout << message.getIdentity().getModule() 
				  << " -> (line " << message.getLine() 
				  << " in " << System::GetFileName(message.getFile()) 
				  << ") (" << message.getCallStack() 
				  << ") : " 
				  << message.getContent() 
				  << '\n';
    }
} // Log