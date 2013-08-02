#ifndef _LISTENING_THREAD_
#define _LISTENING_THREAD_

#include <QThread>
#include <blaspheme/protocol/Session.hpp>

namespace Master
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
        
            void newSlaveConnected(Session);
            
        public slots:

            void listen();
            void stopListen();
            void setListening(bool enabled=true);
            void setNotListening(bool enabled=true);
            void changeListeningPort(int port);
            void changePassword(const QString& password);
            
        private:
            
            bool continue_listen;
            Blaspheme::ConnectionInfo info;
    };
    
} // Master

#endif // _LISTENING_THREAD_
