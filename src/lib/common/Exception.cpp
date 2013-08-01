#include "Exception.hpp"

namespace Common
{
	Exception::Exception(const std::string msgArg)
		: msg(msgArg)
	{
	}

	Exception::~Exception() throw()
	{
	}

	const char * Exception::what() const throw ()
	{
		return msg.c_str();
	}

	void Exception::setMessage(const std::string msgArg)
	{
		msg = msgArg;
	}

} // Common
