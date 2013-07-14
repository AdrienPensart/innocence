#include <iostream>
#include <fstream>
#include <cstdlib>

#ifdef WIN32
	#include <windows.h>
#endif

using namespace std;

#include <common/Logger.hpp>
#include <common/Convert.hpp>

namespace Common
{

    LoggingNetwork::LoggingNetwork(const Network::Host& _debug_server, const Network::Port& _debug_port)
    {
        socket_udp.setDestInfo(_debug_server, _debug_port);
    }

    void LoggingNetwork::update(const std::string& object)
    {
        socket_udp << object;
    }
    
    LoggingFile::LoggingFile(const std::string& _filepath)
    :filepath(_filepath)
    {
        file.open(filepath.c_str());
        if(!file.is_open())
        {
            FATAL_ERROR("Can't open log file.");
        }
    }

    void LoggingFile::update(const std::string& object)
    {
        file << object;
    }
    
    LoggingMessage::LoggingMessage(const std::string& _title)
    :title(_title)
    {
    }

    void LoggingMessage::update(const std::string& object)
    {
		cout << object;
    }

    
    Logger::Logger()
        : tracing(false)
    {
    }
    
    void Logger::trace()
    {
        tracing = true;
    }

    void Logger::enterFunction(const std::string& func)
    {
        functions.push_back(func);
    }

    void Logger::leaveFunction()
    {
        functions.pop_back();
    }

    void Logger::setHeader(const std::string& _header)
    {
        header = _header;
    }
    
    Logger& Logger::operator << (const std::string& object)
    {
        string graph;
        // parcourt des appels de function
        for(unsigned int indexFunctions = 0; indexFunctions != functions.size(); indexFunctions++)
        {
            graph += functions[indexFunctions];
            if(indexFunctions + 1 != functions.size())
            {
                graph += ":";
            }
        }
        notify(header + " -> " + graph + " : " + object + '\n');
        return *this;
    }
    
    ScopedLog::ScopedLog(const std::string& log):
    msg(log)
    {
        LOG << msg + " (started).";
    }

    ScopedLog::~ScopedLog()
    {
        LOG << msg + " (ended).";
    }

    FunctionLog::FunctionLog(const std::string& functionName)
    {
        LOG.enterFunction(functionName);
    }

    FunctionLog::~FunctionLog()
    {
        LOG.leaveFunction();
    }
} // Common

void SHOW_LAST_ERROR()
{
	#ifdef WIN32
		DWORD dw = GetLastError();
		LOG << "Last error = " + to_string(dw);
	#else
		LOG << "Last error = " + to_string(errno);
	#endif
    
}

void FATAL_ERROR(const std::string& msg)
{
	LOG << msg;
	exit(EXIT_FAILURE);
}

void SHOW_FATAL_ERROR(const std::string& msg)
{
#ifdef WIN32
	MessageBoxA(NULL, msg.c_str() , "", 0);
#endif
    FATAL_ERROR(msg);
}

void SHOW_BOX(const std::string& title, const std::string& msg)
{
#ifdef WIN32
    MessageBoxA(NULL, msg.c_str() , title.c_str(), 0);
#else
    LOG << "Boite de message " + title + " : " + msg;
#endif
}
