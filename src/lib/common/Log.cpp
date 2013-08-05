#ifdef WIN32
	#include <windows.h>
#endif

#include <common/Log.hpp>
#include <ctime>

namespace Common
{
	std::string genTime()
	{
		time_t now = time(0);
		const char * dt = ctime(&now);
		return dt;
	}

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

	unsigned int Message::id = 0;

	Message::Message(const std::string& contentArg, const CallStack& callStackArg) : 
		content(contentArg),
		callStack(genCallStack(callStackArg)),
		emittedTime(genTime())
	{
	}

	const std::string& Message::getContent() const
	{
		return content;
	}

	const std::string& Message::getCallStack() const
	{
		return callStack;
	}

	std::string Message::build()
	{
		return "not implemented";
	}

	ExceptionMessage::ExceptionMessage(const std::string& typeArg, const std::string& contentArg, const CallStack& callStackArg) : 
		Message(contentArg, callStackArg),
		type(typeArg)
	{
	}

	std::string ExceptionMessage::build()
	{
		return "not implemented";
	}

	const std::string& ExceptionMessage::getType() const
	{
		return type;
	}

	Audit::Audit(const std::string& moduleArg, const unsigned int& buildIdArg, const std::string& buildDateArg) :
		module(moduleArg),
		buildId(buildIdArg),
		buildDate(buildDateArg),
		startedAt(genTime())
	{
	}

	Audit::~Audit()
	{
	}

	void Audit::addMessage(const Message& message)
	{
		messages.push_back(message);
	}

	std::string Audit::build()
	{
		return "not implemented";
	}

	GlobalAudit::GlobalAudit()
	{
	}

	GlobalAudit::~GlobalAudit()
	{
	}

	std::string GlobalAudit::build()
	{
		return "not implemented";
	}

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
