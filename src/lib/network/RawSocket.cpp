#include "RawSocket.hpp"

#include <ws2tcpip.h>

namespace Network
{
	USHORT checksum(USHORT *buffer, int size)
	{
		unsigned long cksum=0;
		while (size > 1)
		{
			cksum += *buffer++;
			size  -= sizeof(USHORT);
		}

		if (size)
		{
			cksum += *(UCHAR*)buffer;
		}
		cksum = (cksum >> 16) + (cksum & 0xffff);
		cksum += (cksum >>16);
		return (USHORT)(~cksum);
	}

	void RawSocket::acquire()
    {
        if(sockethandle == NOT_ACQUIRED)
        {
			if(proto == TCP)
			{
				sockethandle = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
			}
			else if(proto == UDP)
			{
				sockethandle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			}
			else if(proto == ICMP)
			{
				sockethandle = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
			}
			else
			{
				throw SocketException("invalid protocol type", getDescriptor());
			}

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
