#include "UdpSocket.hpp"
#include "Select.hpp"

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
				throw SocketException(this->getDescriptor(), "UdpSock:AcquireSocket:Socket failed");
            }
        }
    }
    
    int UdpSocket::getBufferDataSize()
    {
        unsigned long size = 0;
        if (ioctlsocket(sockethandle, FIONREAD, &size) != 0)
        {
            throw SocketException(this->getDescriptor(), "TcpClient::Recv::ioctlsocket can't retrieve size of data");
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
            throw SocketException(this->getDescriptor(), "UdpSock::Listen::Bind failed");
        }
    }
    
    int UdpSocket::send(const char * object, int sizeOfObject)
    {
        acquire();
        
        int returnChar = sendto(sockethandle,object,sizeOfObject,0,(Addr *)&attachedAddr,sizeof(sockaddr_in));
        if (returnChar == SOCK_ERROR)
        {
            throw SocketException(this->getDescriptor(), "UdpSock::Send::sendto : SOCK_ERROR");
        }
        return returnChar;
    }
    
    int UdpSocket::send(const char * object, int sizeOfObject, Timeout to)
    {
        acquire();
        
        int returnChar = 0;
        if(SelectSocket::SelectOnWrite(*this, to))
        {
            returnChar = sendto(sockethandle,object,sizeOfObject,0,(Addr *)&attachedAddr,sizeof(sockaddr_in));
            if (returnChar == SOCK_ERROR)
            {
                throw SocketException(this->getDescriptor(), "UdpSock::Send::sendto : SOCK_ERROR");
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
            Deconnection(this->getDescriptor(), "UdpSock::Recv::recvfrom : Deconnection");
        }
        return returnChar;
    }
    
    int UdpSocket::recv(char * object, int sizeOfObject, Timeout to)
    {
        acquire();
        
        int returnChar = 0;
        if(SelectSocket::SelectOnRead(*this, to))
        {
            socklen_t sin_size = sizeof(AddrIn);
            returnChar = recvfrom(sockethandle, object, sizeOfObject, 0,(Addr *)&attachedAddr, &sin_size);
            if(returnChar == SOCK_ERROR)
            {
                Deconnection(this->getDescriptor(), "UdpSock::Recv::recvfrom : Deconnection");
            }
        }
        return returnChar;
    }

}
