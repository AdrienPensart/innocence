#include "SocketException.hpp"

#include <common/Convert.hpp>
#include <log/Log.hpp>

namespace Network
{
	SocketException::SocketException(SocketHandleImpl sock, const std::string& msgArg) : 
		Exception(msgArg + " " + getLastError()),
		socketValue(sock)
	{
		LOG << "SocketException : " + Common::toString(typeid(this).name()) + msgArg + " " + getLastError();
	}
			
	SocketException::~SocketException() throw()
	{
	}
			
	const SocketHandleImpl& SocketException::getValue() const
	{
		return socketValue;
	}
			
	std::string SocketException::getLastError()
	{
		#ifdef WIN32
			return Common::toString(WSAGetLastError());
		#else
			return Common::toString(errno);
		#endif
	}
			
	int SocketException::getCodeError()
	{
		#ifdef WIN32
			return WSAGetLastError();
		#else
			return errno;
		#endif
	}
} // Network
