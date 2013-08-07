#include "SocketException.hpp"

namespace Network
{
	SocketException::SocketException(SocketHandleImpl sock, const std::string& msgArg) : 
		Exception(msgArg + " " + getLastError() + "\n"),
		socketValue(sock)
	{
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
			return toString(WSAGetLastError());
		#else
			return toString(errno);
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
