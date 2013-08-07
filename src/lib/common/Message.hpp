#pragma once

#include <string>

namespace Innocence
{
	class Identity;
}

namespace Common
{
	std::string genTime();

	class Message
	{
		public:
			
			Message(const Innocence::Identity& identity, const std::string& content, const std::string& callStack);
			
			const std::string& getContent() const ;
			const std::string& getCallStack() const ;
			const std::string& getTime() const ;

		private:

			static unsigned int id;
			const Innocence::Identity& identity;
			const std::string content;
			const std::string callStack;
			const std::string emittedTime;
	};

} // Common
