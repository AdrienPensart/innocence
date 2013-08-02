#pragma once

#include <QThread>
#include <network/UdpSocket.hpp>

namespace Debugger
{
	class DebugThread : public QThread
	{
		Q_OBJECT
        
		public:
        
			DebugThread(Network::Port default_port);
			~DebugThread();
			void run();
            
		signals:
        
			void newDebugMsg(QString msg);
            
		private:
        
			Network::UdpSocket server;
			bool terminating;
	};

} // Debugger
