#pragma once

#include <common/NonCopyable.hpp>
#include <common/Convert.hpp>
#include <common/Observable.hpp>
#include <common/Identity.hpp>

#include "LogObserver.hpp"

#include <string>
#include <vector>
#include <map>

namespace Log
{
	typedef std::vector<std::string> CallStack;

	std::string genCallStack(const CallStack& callStack);

    class Lout : public Common::Observable<LogObserver, Message>, public Common::NonCopyable
    {
        public:
            
            void trace();
			static void setIdentity(const Common::Identity& identity);
            void enterFunction(const std::string& func);
            void leaveFunction();
            Lout& operator << (const std::string& object);
			static Lout lout;
			static Lout& setMacro(const std::string& currentLine, const std::string& currentFile);

        private:
            
			Lout();
            bool tracing;
			static std::string currentLine;
			static std::string currentFile;

			static Common::Identity identity;
            CallStack functions;
    };
    
	struct ScopedLog
    {
        ScopedLog(const std::string& log);
        ~ScopedLog();
        const std::string& msg;
    };
    
    struct FunctionLog
    {
        FunctionLog(const std::string& functionName);
        ~FunctionLog();
    };

} // Log

// hack to comment log lines when in release mode
#define COMMENT SLASH(/)
#define SLASH(s) /##s

#define Q(x) #x
#define QUOTE(x) Q(x)

#ifdef INNOCENCE_DEBUG
	#define LOG Log::Lout::lout.setMacro(QUOTE(__LINE__), QUOTE(__FILE__))
	#define TRACE_FUNCTION Log::FunctionLog function_log((__FUNCTION__));
#else
	#define LOG COMMENT
	#define TRACE_FUNCTION COMMENT
#endif

#define CATCH_UNKNOWN_EXCEPTION catch(...){LOG << "Unknown exception";}

//#define SCOPED_LOG(x) Common::ScopedLog scoped_log((x));
//#define FUNC_LOG(x) Common::FunctionLog function_log((x));

