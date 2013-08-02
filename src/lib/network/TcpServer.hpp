#pragma once

#include "TcpSocket.hpp"
#include "TcpClient.hpp"

namespace Network
{
    /**
     * Représente un serveur Tcp
     */
    class TcpServer : public TcpSocket
    {
        public:

            TcpServer();
            TcpServer(const Port& port);
            ~TcpServer();
            
            void listen(const Port argPort, unsigned short int nbSock=1);
            void stopListen();
            bool isListening();
            
            bool isThereConnection(Timeout to=Timeout());
            bool accept(TcpClient& newSock);
            bool accept(TcpClient& newSock, Timeout to);
        
		private:
        
			bool listening;
    };
} // Network
