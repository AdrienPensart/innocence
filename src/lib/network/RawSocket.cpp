#include "RawSocket.hpp"

#ifdef WIN32
#include <ws2tcpip.h>
#include <log/Log.hpp>

namespace Network
{
	RawSocket::RawSocket(int protocolArg) :
		protocol(protocolArg)
	{
	}

	int RawSocket::send(Packet& packet)
	{
		std::vector<char> buffer;
		packet.forge(buffer);
		return send(&buffer[0], buffer.size());
	}

	int RawSocket::send(const char * buffer, size_t length)
	{
		TRACE_FUNCTION
		acquire();
		LOG << "Handle : " + Common::toString(sockethandle) + ", length : " + Common::toString(length);
		return sendto(sockethandle, buffer, length, 0, (SOCKADDR *)&attachedAddr, sizeof(attachedAddr));
	}

	int RawSocket::recv(char * object, int sizeOfObject)
	{
		TRACE_FUNCTION
		acquire();

		socklen_t sin_size = sizeof(AddrIn);
		int returnChar = recvfrom(sockethandle, object, sizeOfObject, 0,(Addr *)&attachedAddr, &sin_size);
		if(returnChar == SOCK_ERROR)
		{
			Deconnection("recvfrom : disconnected", getDescriptor());
		}
		return returnChar;
	}

	int RawSocket::recv(Packet& packet)
	{
		TRACE_FUNCTION
		std::vector<char> buffer;
		buffer.resize(65535);
		int bytesRecv = recv(&buffer[0], buffer.size());
		buffer.resize(bytesRecv);
		packet.feed(buffer);
		return bytesRecv;
	}

	void RawSocket::listen(const Port& argPort)
	{
		TRACE_FUNCTION
		acquire();

		attachedAddr.sin_addr.s_addr = INADDR_ANY;
		attachedAddr.sin_port = htons(argPort);

		int error = bind(sockethandle, (Addr *)&attachedAddr, sizeof(Addr));
		if(error)
		{
			throw SocketException("bind failed", getDescriptor());
		}
	}

	void RawSocket::acquire()
	{
		if(sockethandle == NOT_ACQUIRED)
		{
			sockethandle = socket(AF_INET, SOCK_RAW, protocol);
			if(sockethandle == INVALID_SOCK)
			{
				throw SocketException("socket failed", getDescriptor());
			}
			int optval=1;
			if(setOptions(IPPROTO_IP, IP_HDRINCL, (char *)&optval, sizeof(optval)))
			{
				 throw SocketException("setoptions failed", getDescriptor());
			}
		}
	}
} // Network

#endif