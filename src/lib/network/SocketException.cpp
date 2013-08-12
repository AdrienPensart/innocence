#include "SocketException.hpp"

#include <common/Utility.hpp>
#include <log/Log.hpp>

namespace Network
{
	SocketException::SocketException(const std::string& msgArg, SocketHandleImpl sock, int errorCodeArg) : 
		socketValue(sock),
		errorCode(errorCodeArg)
	{
		std::string msg = "SocketException : " + Common::toString(typeid(this).name()) + msgArg;
		if(sock == 0 && errorCode != 0)
		{
			msg += ", error : " + Common::toString(errorCode);
		}
		else
		{
			msg += ", socket : " + Common::toString(sock) + ", error : " + getLastError();
		}
		setMessage(msg);
		LOG << what();
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
