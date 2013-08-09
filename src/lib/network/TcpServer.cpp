#include "TcpServer.hpp"
#include "SelectSet.hpp"
#include "Interface.hpp"

namespace Network
{
    using namespace BSDSocket;
    
    TcpServer::TcpServer()
    :listening(false)
    {
    }
    
    TcpServer::TcpServer(const Port& port)
    :listening(false)
    {
        listen(port);
    }
    
    TcpServer::~TcpServer()
    {
		stopListen();
    }
    
    bool TcpServer::isListening()
    {
        return listening;
    }
    
    void TcpServer::listen(const Port argPort, unsigned short int nbSock)
    {
        // est-on déjà en ecoute ?
        if(listening)
        {
            throw SocketException("listen : already listening", getDescriptor());
        }
        
        // configuration des options d'écoute
        attachedAddr.sin_addr.s_addr = INADDR_ANY;
        attachedAddr.sin_port = htons(argPort);
        
        // on s'assure que le l'on est pas deja en ecoute (si on veut changer de port)
        if(sockethandle != NOT_ACQUIRED)
        {
            stopListen();
        }
        else
        {
            // on acquiert un handle
            acquire();
            
            // puis on le configure pour que l'on puisse se binder plusieurs
            // fois de suite sur le port
            static const int reuse = -1;
            if(setOptions(SOL_SOCKET,SO_REUSEADDR,(int *)&reuse,sizeof(reuse)))
            {
                throw SocketException("Option SO_REUSEADDR can't be set", getDescriptor());
            }
        }
        
        if(bind(sockethandle, (Addr *)&attachedAddr, sizeof(Addr)) != 0)
        {
            throw SocketException("bind failed" + getDescriptor());
        }
        if (bsd_listen(sockethandle, nbSock) <= SOCK_ERROR)
        {
            throw SocketException("listen failed", getDescriptor());
        }
        
        listening = true;
    }
	
	void TcpServer::stopListen()
	{
	    if(listening)
	    {
	        // resume opened connections
	        bsd_listen(sockethandle, 0);
            close();
            listening = false;
	    }
	}
	
    bool TcpServer::isThereConnection(Timeout to)
    {
        return SelectSet::SelectOnRead(*this, to);
    }
    
    bool TcpServer::accept(TcpClient& client_sock)
    {
        // si le serveur n'est pas en mode ecoute, on lance une exception
        if(!listening)
        {
            throw SocketException("not listening", getDescriptor());
        }
            
        // test preliminaire de validite
        test();
        
        // on utilise une variable afin de passer par un pointeur
        socklen_t sin_size = sizeof(AddrIn);
        AddrIn hisAddr;

        SocketHandleImpl handle = bsd_accept(sockethandle, (Addr *)&hisAddr, &sin_size);
        if((handle) == INVALID_SOCK)
        {
            // pas de connexion
            return false;
        }
        else
        {
            // on met a jour la socket cliente
            client_sock.setDescriptor(handle);
            // connexion effectuee on copie les informations de connexion
            client_sock.setAddr(hisAddr);
        }
        return true;
    }
    
    bool TcpServer::accept(TcpClient& client_sock, Timeout to)
    {
        if (SelectSet::SelectOnRead(*this, to))
        {
            return accept(client_sock);
        }
        return false;
    }
}
