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

	class Message
	{
		public:
			
			Message(const std::string& content, const CallStack& callStack);

			const std::string& getContent() const ;
			const std::string& getCallStack() const ;
			const std::string& getTime() const ;
			virtual std::string build();

		private:

			static unsigned int id;
			const std::string content;
			const std::string callStack;
			const std::string emittedTime;
	};

	// for error messages
	class ExceptionMessage : public Message
	{
		public:
			
			ExceptionMessage(const std::string& typeArg, const std::string& content, const CallStack& callStack);
			const std::string& getType() const;
			virtual std::string build();

		private:

			std::string type;
	};

	class Audit
	{
		public:
			
			Audit(const std::string& module, const unsigned int& buildId, const std::string& buildDate);
			virtual ~Audit();
			void addMessage(const Message& message);
			virtual std::string build();

		private:

			const unsigned int buildId;
			const std::string buildDate;
			const std::string module;
			std::vector<Message> messages;
			const std::string startedAt;
	};

	class GlobalAudit
	{
		public:
			
			GlobalAudit();
			virtual ~GlobalAudit();
			virtual std::string build();

		private:

			std::vector<Audit> audits;
			time_t startedAt;
	};

    class Log : public Observable<LogObserver, std::string>, public Common::NonCopyable
    {
        public:
            
            void trace();
            void setHeader(const std::string& header);
            void enterFunction(const std::string& func);
            void leaveFunction();
			void sendRaw(const std::string& object);
            Log& operator << (const std::string& object);
			static Log lout;

        private:
            
			Log();
            bool tracing;
            std::string header;
            CallStack functions;
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
