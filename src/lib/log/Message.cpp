#include "Message.hpp"
#include "Log.hpp"
#include <common/Utility.hpp>

namespace Log
{
	namespace Private
	{
		const char * marker = "M#M";
	}

	Message::Message()
	{
	}

	Message::Message(const Common::Identity& identityArg, const std::string& contentArg, const std::string& callStackArg, const std::string& lineArg, const std::string& fileArg) :
		identity(identityArg),
		content(contentArg),
		callStack(callStackArg),
		line(lineArg),
		file(fileArg),
		emittedTime(Common::currentDate())
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

	const std::string& Message::getLine() const
	{
		return line;
	}

	const std::string& Message::getFile() const
	{
		return file;
	}

	const std::string& Message::getTime() const
	{
		return emittedTime;
	}

	Common::Identity& Message::getIdentity()
	{
		return identity;
	}

	const Common::Identity& Message::getIdentity() const
	{
		return identity;
	}

	std::string Message::serialize() const
	{
		return identity.serialize() + Private::marker + content + Private::marker + callStack + Private::marker + emittedTime + Private::marker + line + Private::marker + file;
	}

	void Message::deserialize(const std::string& buffer)
	{
		std::string bufferCopy = buffer;
		std::vector<std::string> output;
		Common::split(bufferCopy, Private::marker, output);
		identity.deserialize(output[0]);
		content = output[1];
		callStack = output[2];
		emittedTime = output[3];
		line = output[4];
		file = output[5];
	}
} // Log
