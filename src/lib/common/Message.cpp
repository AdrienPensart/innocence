#include "Message.hpp"
#include "Log.hpp"

#include <ctime>

namespace Common
{
	std::string genTime()
	{
		time_t now = time(0);
		const char * dt = ctime(&now);
		return dt;
	}

	unsigned int Message::id = 0;

	Message::Message(const std::string& contentArg, const std::string& callStackArg) : 
		content(contentArg),
		callStack(callStackArg),
		emittedTime(genTime())
	{
	}

	Message::~Message()
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

	std::string Message::build() const
	{
		return LOG.getHeader() + " -> " + getCallStack() + " : " + getContent() + "\n";
	}

	ExceptionMessage::ExceptionMessage(const std::string& typeArg, const std::string& contentArg, const std::string& callStackArg) : 
		Message(contentArg, callStackArg),
		type(typeArg)
	{
	}

	std::string ExceptionMessage::build() const
	{
		return LOG.getHeader() + " -> " + getCallStack() + " : " + getContent() + "\n";
	}

	const std::string& ExceptionMessage::getType() const
	{
		return type;
	}
} // Common
