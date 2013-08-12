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

	LogToCollector::LogToCollector(const Network::Host& loggerIp, const Network::Port& loggerPort) : 
		LogToNetwork(loggerIp, loggerPort)
	{
	}

    LogToFile::LogToFile(const std::string& filepathArg) : 
		filepath(filepathArg),
		disabled(false)
    {
        file.open(filepath.c_str(), std::ofstream::out | std::ofstream::trunc);
        if(!file.is_open())
        {
			disabled = true;
            LOG << "Unable to create or open log file " + filepath;
        }
    }

    void LogToFile::update(const Message& message)
    {
		if(!disabled)
		{
			file  << message.getIdentity().getModule()
  				  << " on " << message.getIdentity().getOs()
				  << " -> (" << message.getTime() << ", " << message.getLine() 
				  << " in " << System::GetFileName(message.getFile()) 
				  << ") (" << message.getCallStack() 
				  << ") : " 
				  << message.getContent() 
				  << '\n';
		}
    }
    
    LogToConsole::LogToConsole(const std::string& _title)
    :title(_title)
    {
    }

    void LogToConsole::update(const Message& message)
    {
		std::cout << message.getIdentity().getModule() 
				  << " on " << message.getIdentity().getOs()
				  << " -> (" << message.getTime() << ", line " << message.getLine() 
				  << " in " << System::GetFileName(message.getFile()) 
				  << ") (" << message.getCallStack() 
				  << ") : " 
				  << message.getContent() 
				  << '\n';
    }
} // Log
