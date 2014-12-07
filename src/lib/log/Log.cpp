#include <common/WindowsWrapper.hpp>
#include <common/Identity.hpp>
#include <network/Service.hpp>
#include <iostream>
#include "Log.hpp"
#include "Message.hpp"

namespace Log
{
	std::string genCallStack(const CallStack& callStack)
	{
		std::string graph;
		for(unsigned int indexFunctions = 0; indexFunctions != callStack.size(); indexFunctions++)
		{
			graph += callStack[indexFunctions];
			if(indexFunctions + 1 != callStack.size())
			{
				graph += ":";
			}
		}
		return graph;
	}

	Common::Identity Lout::identity;
	std::string Lout::currentLine = QUOTE(__LINE__);
	std::string Lout::currentFile = __FILE__;
	Lout Lout::lout;

	Lout::Lout() :
		tracing(true),
		warningMessage(true)
	{
	}

	void Lout::setTracing(bool trace)
	{
		tracing = trace;
	}

	void Lout::enterFunction(const std::string& func)
	{
		functions.push_back(func);
	}

	void Lout::leaveFunction()
	{
		functions.pop_back();
	}

	void Lout::setIdentity(const Common::Identity& identityArg)
	{
		identity = identityArg;
	}

	Lout& Lout::setMacro(const std::string& currentLineArg, const std::string& currentFileArg)
	{
		currentLine = currentLineArg;
		currentFile = currentFileArg;
		return lout;
	}

	Lout& Lout::operator << (const std::string& object)
	{
		std::string graph = genCallStack(functions);
		Message message(identity, object, graph, currentLine, currentFile);
		message.getIdentity().setIp(Network::HostInfo::getLocalIp());
		if(!isObserved() && warningMessage)
		{
			std::cout << "Warning : no output observer registered.\n";
			warningMessage = false;
		}
		notify(message);
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
} // Log

/*

void SHOW_BOX(const std::string& title, const std::string& msg)
{
#ifdef WIN32
	MessageBoxA(NULL, msg.c_str() , title.c_str(), 0);
#else
	LOG << "Message Box " + title + " : " + msg;
#endif
}

*/
