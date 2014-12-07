#include "Socket.hpp"
#include "Interface.hpp"
#include <cstring>

namespace Network
{
	// Constructeurs et destructeur
	Socket::Socket() :
		blocking_status(true)
	{
		// initialisation a zero de la structure d'adresse
		memset(&attachedAddr,0,sizeof(AddrIn));
		attachedAddr.sin_family = AF_INET;
	}

	// activation de l'algorithme de Nagle, etc...
	int Socket::setOptions(const int& level,const int& optname,
						   const void *optval, const socklen_t& optlen)
	{
		return setsockopt(sockethandle, level, optname,(char *)&optval,sizeof(optlen));
	}

	int Socket::getOptions(int level, int option_name, char * option_value, socklen_t *option_len)
	{
		return getsockopt(sockethandle, level, option_name, option_value, option_len);
	}

	const Host Socket::getIp(void)
	{
		return inet_ntoa(attachedAddr.sin_addr);
	}

	const Port Socket::getPort(void)
	{
		return ntohs(attachedAddr.sin_port);
	}

	int Socket::getPeerName(AddrIn& addr)
	{
		socklen_t sizeAddr = sizeof(addr);
		return getpeername(sockethandle,(Addr *)&addr, &sizeAddr);
	}

	void Socket::setDestInfo(const Host& argIp, const Port& argPort)
	{
		setDestIp(argIp);
		setDestPort(argPort);
	}

	void Socket::setAddr(const AddrIn& Info)
	{
		attachedAddr = Info;
	}

	const AddrIn& Socket::getAddr() const
	{
		return attachedAddr;
	}

	void Socket::setDestIp(const Host& argIp)
	{
		attachedAddr.sin_addr.s_addr = inet_addr(argIp.c_str());
	}

	void Socket::setDestPort(const Port& argPort)
	{
		attachedAddr.sin_port = htons(argPort);
	}

	void Socket::close(void)
	{
		if(sockethandle != NOT_ACQUIRED)
		{
			if(BSDSocket::bsd_close(sockethandle) == SOCKET_ERROR)
			{
				throw SocketException("close failed", this->getDescriptor());
			}
			sockethandle = NOT_ACQUIRED;
		}
	}

	int Socket::setBlocking(const bool blocking)
	{
		int result = 0;
		#ifdef WIN32
			unsigned long arg = 0;

			if (blocking == true)
			{
				arg = 0;
				blocking_status = true;
			}
			else
			{
				blocking_status = false;
				arg = 1;
			}
			result = ioctlsocket (sockethandle, FIONBIO, &arg);
		#else
			int flags = fcntl (sockethandle, F_GETFL);

			if (blocking == true)
			{
				result = fcntl (sockethandle, F_SETFL, flags & ~O_NONBLOCK);
				blocking_status = true;
			}
			else
			{
				result = fcntl (sockethandle, F_SETFL, flags | O_NONBLOCK);
				blocking_status = false;
			}
		#endif
		return result;
	}

	bool Socket::setSendBufSize(int sendBufSize)
	{
		if (setsockopt(sockethandle,SOL_SOCKET,SO_SNDBUF,(char *)&sendBufSize,sizeof(sendBufSize)) == SOCK_ERROR )
			return false;
		return true;
	}

	bool Socket::setReceiveBufSize(int receiveBufSize)
	{
		if ( setsockopt(sockethandle,SOL_SOCKET,SO_RCVBUF,(char *)&receiveBufSize,sizeof(receiveBufSize)) == SOCK_ERROR )
			return false;
		return true;
	}

	int Socket::getSendBufSize()
	{
		int sendBuf;
		int myOptionLen = sizeof(sendBuf);

		if ( getsockopt(sockethandle,SOL_SOCKET,SO_SNDBUF,(char *)&sendBuf,(socklen_t*)&myOptionLen) == SOCK_ERROR )
			return false;
		return sendBuf;
	}

	int Socket::getReceiveBufSize()
	{
		int rcvBuf;
		int myOptionLen = sizeof(rcvBuf);

		if ( getsockopt(sockethandle,SOL_SOCKET,SO_RCVBUF,(char *)&rcvBuf,(socklen_t*)&myOptionLen) == SOCK_ERROR )
			return false;

		return rcvBuf;
	}
}
