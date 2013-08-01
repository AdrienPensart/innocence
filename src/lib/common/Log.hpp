#ifndef _LOGGER_HPP_
#define _LOGGER_HPP_

#include "Singleton.hpp"
#include "Convert.hpp"
#include "Observable.hpp"
#include "LogObserver.hpp"

#include <string>
#include <vector>

namespace Common
{
    class Log : public Singleton<Log>, public Observable<LogObserver, std::string>
    {
        public:
            
            void trace();
            void setHeader(const std::string& header);
            void enterFunction(const std::string& func);
            void leaveFunction();
			void sendRaw(const std::string& object);
            Log& operator << (const std::string& object);
        
        protected:

            Log();

        private:
            
            friend class Singleton<Log>;
            bool tracing;
            std::string header;
            std::vector<std::string> functions;
    };
    
	#define LOG	Common::Log::instance()
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

} /* Common */

#define SCOPED_LOG(x) Common::ScopedLog scoped_log((x));
#define FUNC_LOG(x) Common::FunctionLog function_log((x));
#define LOG_THIS_FUNCTION Common::FunctionLog function_log((__FUNCTION__));

#endif // _LOGGER_HPP_
