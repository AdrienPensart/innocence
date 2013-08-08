#include "Message.hpp"
#include "Log.hpp"
#include <ctime>
#include <algorithm>

namespace Log
{
	namespace Private
	{
		const char * marker = "M#M";
	}

	std::string genTime()
	{
		time_t now;
		time(&now);
		std::string mytime = ctime(&now);
		mytime.erase(std::remove(mytime.begin(), mytime.end(), '\n'), mytime.end());
		return mytime;
	}

	unsigned int Message::id = 0;

	Message::Message()
	{
	}

	Message::Message(const Common::Identity& identityArg, const std::string& contentArg, const std::string& callStackArg) : 
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

	const std::string& Message::getTime() const
	{
		return emittedTime;
	}

	const Common::Identity& Message::getIdentity() const
	{
		return identity;
	}

	std::string Message::serialize() const 
	{
		return identity.serialize() + Private::marker + content + Private::marker + callStack + Private::marker + emittedTime;
	}
	
	void Message::deserialize(const std::string& buffer)
	{
		std::string bufferCopy = buffer;
		std::vector<std::string> output;
		Serializable::split(bufferCopy, Private::marker, output);
		identity.deserialize(output[0]);
		content = output[1];
		callStack = output[2];
		emittedTime = output[3];
	}
} // Log
