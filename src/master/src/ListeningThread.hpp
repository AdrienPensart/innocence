//!
//!     Kaleidoscalp, all rights reserved.
//!

#ifndef _LISTENING_THREAD_HPP_
#define _LISTENING_THREAD_HPP_

#include <QThread>
#include <network/TcpServer.hpp>
#include <network/TcpClient.hpp>
#include <blaspheme/protocol/Session.hpp>

namespace TheSleeper
{
    using Network::TcpClient;
    using Blaspheme::Session;
    
    class ListeningThread : public QThread
    {
        Q_OBJECT
        enum {MAX_SIMUL_SOCK = 20};
        
        public:
        
            ListeningThread(int port_value=0, const QString& pass_value="", QObject * parent=0);
            virtual ~ListeningThread();
            
            void run();
            bool isListening();
            
        signals:
        
            void newClientConnected(Session);
            
        public slots:

            void listen();
            void stopListen();
            void setListening(bool enabled=true);
            void setNotListening(bool enabled=true);
            void changeListeningPort(int port);
            void changePassword(const QString& password);
            
        private:
            
			Session * session;
            bool continue_listen;
            Blaspheme::ConnectionInfo info;
    };
    
} // TheSleeper

#endif // _LISTENING_THREAD_HPP_
