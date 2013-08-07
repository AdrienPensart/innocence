#ifdef WIN32
	#include <windows.h>
#endif

#include "Log.hpp"
#include "Message.hpp"
#include <Identity.hpp>

namespace Common
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

	Innocence::Identity Log::identity;
	Log Log::lout;

    Log::Log()
        : tracing(false)
    {
    }
    
    void Log::trace()
    {
        tracing = true;
    }

    void Log::enterFunction(const std::string& func)
    {
        functions.push_back(func);
    }

    void Log::leaveFunction()
    {
        functions.pop_back();
    }

    void Log::setIdentity(const Innocence::Identity& identityArg)
    {
        identity = identityArg;
    }

    Log& Log::operator << (const std::string& object)
    {
        std::string graph = genCallStack(functions);
		Message message(identity, object, graph);
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
} // Common

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
