#pragma once

#include "NonCopyable.hpp"
#include "Convert.hpp"
#include "Observable.hpp"
#include "LogObserver.hpp"
#include <Identity.hpp>

#include <string>
#include <vector>
#include <map>

namespace Innocence
{
	class Identity;
}

namespace Common
{
	typedef std::vector<std::string> CallStack;

	std::string genCallStack(const CallStack& callStack);

    class Log : public Observable<LogObserver, Message>, public Common::NonCopyable
    {
        public:
            
            void trace();
			static void setIdentity(const Innocence::Identity& identity);
            void enterFunction(const std::string& func);
            void leaveFunction();
            Log& operator << (const std::string& object);
			static Log lout;

        private:
            
			Log();
            bool tracing;
			static Innocence::Identity identity;
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

} // Common

// hack to comment log lines when in release mode
#define COMMENT SLASH(/)
#define SLASH(s) /##s

#ifdef INNOCENCE_DEBUG
	#define LOG Common::Log::lout
	#define TRACE_FUNCTION Common::FunctionLog function_log((__FUNCTION__));
#else
	#define LOG COMMENT
	#define TRACE_FUNCTION COMMENT
#endif

#define CATCH_UNKNOWN_EXCEPTION catch(...){LOG << "Unknown exception";}

//#define SCOPED_LOG(x) Common::ScopedLog scoped_log((x));
//#define FUNC_LOG(x) Common::FunctionLog function_log((x));

