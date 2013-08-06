#pragma once

#include <string>

namespace Common
{
	std::string genTime();

	class Message
	{
		public:
			
			Message(const std::string& content, const std::string& callStack);
			virtual ~Message();

			const std::string& getContent() const ;
			const std::string& getCallStack() const ;
			const std::string& getTime() const ;
			virtual std::string build() const ;

		private:

			static unsigned int id;
			const std::string content;
			const std::string callStack;
			const std::string emittedTime;
	};

	class ExceptionMessage : public Message
	{
		public:
			
			ExceptionMessage(const std::string& typeArg, const std::string& content, const std::string& callStack);
			const std::string& getType() const;
			virtual std::string build() const;

		private:

			// type of exception
			std::string type;
	};

} // Common
