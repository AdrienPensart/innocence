#include "Message.hpp"
#include "Log.hpp"

#include <ctime>

#include <Identity.hpp>

namespace Common
{
	std::string genTime()
	{
		time_t now = time(0);
		const char * dt = ctime(&now);
		return dt;
	}

	unsigned int Message::id = 0;

	Message::Message(const Innocence::Identity& identityArg, const std::string& contentArg, const std::string& callStackArg) : 
		identity(identityArg),
		content(contentArg),
		callStack(callStackArg),
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

} // Common
