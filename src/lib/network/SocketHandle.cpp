#include "SocketHandle.hpp"
#include "SocketException.hpp"

namespace Network
{
    
#ifdef WIN32

    SocketHandle::WSAInitializer SocketHandle::wsai;

    SocketHandle::WSAInitializer::WSAInitializer()
    {
		int wsaReturn = WSAStartup(MAKEWORD (2, 2),&m_wsadata);
        if (wsaReturn)
        {
            throw WinsockInitExcept("Unable to initialize Winsock", 0,  wsaReturn);
        }
    }
    
    SocketHandle::WSAInitializer::~WSAInitializer()
    {
        WSACleanup();
    }
#endif

    // dans le constructeur par defaut, aucun socket systeme n'est demande
    SocketHandle::SocketHandle():sockethandle(NOT_ACQUIRED)
    {
    }

    SocketHandle::SocketHandle(const SocketHandle& handle)
    {
        sockethandle = handle.sockethandle;
    }

    SocketHandle::~SocketHandle()
    {
    }

    const SocketHandleImpl& SocketHandle::getDescriptor() const 
    {
        return sockethandle;
    }
    
    void SocketHandle::setDescriptor(SocketHandleImpl& s)
    {
        sockethandle = s;
    }
    
    // Permet de verifier la validite d'un socket
    bool SocketHandle::isValid() const
    {
        // un socket valide est un socket enregistre au systeme
        // et non dans un etat de deconnexion
        return (sockethandle == INVALID_SOCK || sockethandle == NOT_ACQUIRED)
        ? false
        : true;
    }

    // lance une exception si la socket n'est pas valide
    void SocketHandle::test(void)
    {
        if(!isValid())
        {
            throw InvalidSock("validity test failed", getDescriptor());
        }
    }
}

