#include "Exception.hpp"
#include "Log.hpp"

#include <typeinfo>

namespace Common
{
	Exception::Exception(const std::string msgArg)
		: msg(msgArg)
	{
		// automatic logging
		LOG << "Exception : " + toString(typeid(this).name()) + msgArg;
	}

	Exception::~Exception() throw()
	{
	}

	const char * Exception::what() const throw ()
	{
		return msg.c_str();
	}
	/*
	void Exception::setMessage(const std::string msgArg)
	{
		msg = msgArg;
	}
	*/
} // Common
