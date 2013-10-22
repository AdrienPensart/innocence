#include "UdpSocket.hpp"
#include "SelectSet.hpp"

#include <string>
using std::string;

namespace Network
{
    UdpSocket::UdpSocket(const Port& argPort)
    {
        listen(argPort);
    }
    
    UdpSocket::UdpSocket()
    {
    }
    
    void UdpSocket::acquire()
    {
        if(sockethandle == NOT_ACQUIRED)
        {
            sockethandle = socket(AF_INET,SOCK_DGRAM,0);
            if(sockethandle == INVALID_SOCK)
            {
				throw SocketException("socket failed", getDescriptor());
            }
        }
    }
    
    int UdpSocket::getBufferDataSize()
    {
        unsigned long size = 0;
        if (ioctlsocket(sockethandle, FIONREAD, &size) != 0)
        {
            throw SocketException("ioctlsocket can't retrieve buffer size", getDescriptor());
        }
        return size;
    }
    
    void UdpSocket::listen(const Port& argPort)
    {
        acquire();

        // initialisation des donnees :
        attachedAddr.sin_addr.s_addr = INADDR_ANY;
        attachedAddr.sin_port = htons(argPort);
		
        int error = bind(sockethandle, (Addr *)&attachedAddr, sizeof(Addr));
        if(error)
        {
            throw SocketException("bind failed", getDescriptor());
        }
    }
    
    int UdpSocket::send(const char * object, int sizeOfObject)
    {
        acquire();
        
        int returnChar = sendto(sockethandle,object,sizeOfObject,0,(Addr *)&attachedAddr,sizeof(sockaddr_in));
        if (returnChar == SOCK_ERROR)
        {
            throw SocketException("sendto failed", getDescriptor());
        }
        return returnChar;
    }
    
    int UdpSocket::send(const char * object, int sizeOfObject, Timeout to)
    {
        acquire();
        
        int returnChar = 0;
        if(SelectSet::SelectOnWrite(*this, to))
        {
            returnChar = sendto(sockethandle,object,sizeOfObject,0,(Addr *)&attachedAddr,sizeof(sockaddr_in));
            if (returnChar == SOCK_ERROR)
            {
                throw SocketException("sendto failed", getDescriptor());
            }
        }
        return returnChar;
    }
    
    int UdpSocket::recv(char * object, int sizeOfObject)
    {
        acquire();

        socklen_t sin_size = sizeof(AddrIn);
        int returnChar = recvfrom(sockethandle, object, sizeOfObject, 0,(Addr *)&attachedAddr, &sin_size);
        if(returnChar == SOCK_ERROR)
        {
            Deconnection("recvfrom : disconnected", getDescriptor());
        }
        return returnChar;
    }
    
    int UdpSocket::recv(char * object, int sizeOfObject, Timeout to)
    {
        acquire();
        
        int returnChar = 0;
        if(SelectSet::SelectOnRead(*this, to))
        {
            socklen_t sin_size = sizeof(AddrIn);
            returnChar = recvfrom(sockethandle, object, sizeOfObject, 0,(Addr *)&attachedAddr, &sin_size);
            if(returnChar == SOCK_ERROR)
            {
                Deconnection("recvfrom : disconnected", getDescriptor());
            }
        }
        return returnChar;
    }

	int UdpSocket::send(const std::string& object)
	{
		return Stream::send(object);
	}

    int UdpSocket::send(const std::string& object, Timeout to)
	{
		return Stream::send(object, to);
	}
            
    int UdpSocket::recv(std::string& object, char delimiter, bool include)
	{
		return Stream::recv(object, delimiter, include);
	}

    string UdpSocket::recv()
	{
		return Stream::recv();
	}

	int UdpSocket::recv(std::string& object)
	{
		return Stream::recv(object);
	}

	int UdpSocket::recv(string& object, Timeout to)
	{
		return Stream::recv(object, to);
	}

}
