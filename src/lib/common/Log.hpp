#pragma once

#include "NonCopyable.hpp"
#include "Convert.hpp"
#include "Observable.hpp"
#include "LogObserver.hpp"

#include <string>
#include <vector>
#include <map>

namespace Common
{
	typedef std::vector<std::string> CallStack;

	std::string genCallStack(const CallStack& callStack);

    class Log : public Observable<LogObserver, Message>, public Common::NonCopyable
    {
        public:
            
            void trace();
            void setHeader(const std::string& header);
			const std::string& getHeader()const;
            void enterFunction(const std::string& func);
            void leaveFunction();

            Log& operator << (const std::string& object);
			Log& operator << (Exception& e);

			static Log lout;

        private:
            
			Log();
            bool tracing;
            std::string header;
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

