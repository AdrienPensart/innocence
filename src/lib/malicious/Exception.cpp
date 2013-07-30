#include "Exception.hpp"

namespace Malicious
{
	Exception::Exception(const std::string& argMsgError)
	{
		msgError = argMsgError;
	}

	Exception::~Exception() throw()
	{
	}

	const char * Exception::what() const throw ()
	{
		return msgError.c_str();
	}
}
