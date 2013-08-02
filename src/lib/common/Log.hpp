#pragma once

#include "NonCopyable.hpp"
#include "Convert.hpp"
#include "Observable.hpp"
#include "LogObserver.hpp"

#include <string>
#include <vector>

namespace Common
{
    class Log : public Observable<LogObserver, std::string>, public Common::NonCopyable
    {
        public:
            
			Log();
            void trace();
            void setHeader(const std::string& header);
            void enterFunction(const std::string& func);
            void leaveFunction();
			void sendRaw(const std::string& object);
            Log& operator << (const std::string& object);
			static Log lout;

        private:
            
            bool tracing;
            std::string header;
            std::vector<std::string> functions;
    };
    
	#define COMMENT SLASH(/)
	#define SLASH(s) /##s

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

#ifdef INNOCENCE_DEBUG
	#define LOG Common::Log::lout
#else
	#define LOG COMMENT
#endif

#define SCOPED_LOG(x) Common::ScopedLog scoped_log((x));
#define FUNC_LOG(x) Common::FunctionLog function_log((x));
#define LOG_THIS_FUNCTION Common::FunctionLog function_log((__FUNCTION__));
