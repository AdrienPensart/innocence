#include "TcpClient.hpp"
#include "Interface.hpp"

namespace Network
{
    using namespace BSDSocket;
    
    bool TcpClient::setKeepAlive(int aliveToggle)
    {
        if (setsockopt(sockethandle,SOL_SOCKET,SO_KEEPALIVE,(char *)&aliveToggle,sizeof(aliveToggle)) == SOCK_ERROR)
        {
            return false;
        }
        return true;
    } 
        
    int TcpClient::getBufferDataSize()
    {
        unsigned long size = 0;
        if (ioctlsocket(sockethandle, FIONREAD, &size) != 0)
        {
            throw SocketException(this->getDescriptor(), "TcpClient::Recv::ioctlsocket can't retrieve size of data");
        }
        return size;
    }
    
    bool TcpClient::connect(const Host& argIp, const Port& argPort)
    {
        // on demande un socket au systeme seulement si l'on en possede pas : 
        acquire();
        
        // on remplit les informations de connexion
        attachedAddr.sin_addr.s_addr = inet_addr(argIp.c_str());
        attachedAddr.sin_port = htons(argPort);
        
        // connexion effective
        if(bsd_connect(sockethandle,(Addr *)&attachedAddr, sizeof(Addr)))
        {
            // echec
            return false;
        }
        // connexion ok
        return true;
    }

    #ifdef _WINDOWS_
    bool TcpClient::connect(const std::string& argIp, const Port argPort, Timeout to)
    {
        acquire();
        unblock();
        
        // on remplit les informations de connexion
        attachedAddr.sin_addr.s_addr = inet_addr(argIp.c_str());
        attachedAddr.sin_port = htons(argPort);
        
        int res = bsd_connect(sockethandle,(Addr *)&attachedAddr, sizeof(Addr));
        int valopt = 0;
        bool ret = false;
        
        if (res)
        {
            if (WSAGetLastError() == WSAEINPROGRESS || WSAGetLastError() == WSAEWOULDBLOCK)
            {
                timeval tv = to.c_struct();
                fd_set myset; 
                FD_ZERO(&myset); 
                FD_SET(sockethandle, &myset); 
                
                if (select(sockethandle+1, NULL, &myset, NULL, &tv) > 0)
                {
                    socklen_t lon = sizeof(int); 
                    getsockopt(sockethandle, SOL_SOCKET, SO_ERROR, (char*)(&valopt), &lon); 
                    if (!valopt)
                    { 
                        ret = true;
                    }
                }
            } 
        }
        else
        {
            ret = true;
        }
        
        block();
        return ret;
    }
    #endif
    
    void TcpClient::reset()
    {
        disconnect();
        close();
    }
    
    void TcpClient::disconnect()
    {
        if(sockethandle != NOT_ACQUIRED)
        {
            if(shutdown(sockethandle,2) == SOCKET_ERROR)
            {
                throw SocketException(this->getDescriptor(), "Impossible de deconnecter le socket");
            }
        }
    }
    
    int TcpClient::send(const char * object, int sizeOfObject)
    {
        if(!sizeOfObject)
        {
            throw SocketException(this->getDescriptor(), "TcpClient::Send : size of object is 0");
        }
        
        int returnChar = bsd_send(sockethandle, object, sizeOfObject, 0);
        if(returnChar == SOCK_ERROR)
        {
            throw Deconnection(this->getDescriptor(), "TcpClient::Send : Deconnection");
        }
        return returnChar;
    }
    
    int TcpClient::send(const char * object, int sizeOfObject, Timeout to)
    {
        if(!sizeOfObject)
        {
            throw SocketException(this->getDescriptor(), "TcpClient::Send : size of object is 0");
        }

        if(SelectSocket::SelectOnWrite(*this, to))
        {
            int returnChar = bsd_send(sockethandle, object, sizeOfObject, 0);
            if(returnChar == SOCK_ERROR)
            {
				throw Deconnection(this->getDescriptor(), "TcpClient::Send : Deconnection");
            }
            return returnChar;
        }
        return 0;
    }
    
    int TcpClient::recv(char * object, int sizeOfObject)
    {
        if(!sizeOfObject)
        {
            throw SocketException(this->getDescriptor(), "TcpClient::Recv : size of object is 0");
        }

        int returnChar = bsd_recv(sockethandle, object, sizeOfObject, 0);
        if(returnChar == SOCK_ERROR)
        {
            throw Deconnection(this->getDescriptor(), "TcpClient::Recv : Deconnection");
        }
        return returnChar;
    }
    
    int TcpClient::recv(char * object, int sizeOfObject, Timeout to)
    {
        if(!sizeOfObject)
        {
            throw SocketException(this->getDescriptor(), "TcpClient::Recv : size of object is 0");
        }
        if(SelectSocket::SelectOnRead(*this, to))
        {
            int returnChar = bsd_recv(sockethandle, object, sizeOfObject, 0);
            if(returnChar == SOCK_ERROR)
            {
                throw Deconnection(this->getDescriptor(), "TcpClient::Recv : Deconnection");
            }
            return returnChar;
        }
        return 0;
    }
    
} /* Network */