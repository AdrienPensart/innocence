#include "Exception.hpp"

namespace Network
{
	Exception::Exception(const std::string msg)
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

} // Network
