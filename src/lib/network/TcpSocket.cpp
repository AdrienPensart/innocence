#include "TcpSocket.hpp"

namespace Network
{    
    void TcpSocket::acquire()
    {
        if(sockethandle == NOT_ACQUIRED)
        {
            sockethandle = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
            if(sockethandle == INVALID_SOCK)
            {
				throw SocketException("socket failed", getDescriptor());
            }
        }
    }
} // Network
