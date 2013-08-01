#ifdef WIN32
	#include <windows.h>
#endif

#include <common/Log.hpp>

namespace Common
{
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

    void Log::setHeader(const std::string& _header)
    {
        header = _header;
    }
    
    Log& Log::operator << (const std::string& object)
    {
        std::string graph;
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
    
	void Log::sendRaw(const std::string& object)
	{
		notify(object);
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
