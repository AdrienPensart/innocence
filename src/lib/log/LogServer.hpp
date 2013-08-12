#pragma once

#include <network/TcpServer.hpp>
#include <network/SelectSet.hpp>

#include <common/Observable.hpp>
#include <common/NonCopyable.hpp>

#include "LogObserver.hpp"
#include "Message.hpp"

namespace Log
{
	class LogServer : public Common::Observable<LogObserver, Message>, public Common::NonCopyable
	{
		public:

			LogServer(const Network::Port& port, Network::Timeout deadlineArg=Network::Timeout(1));
			void run();
			void stop();

		private:

			bool interrupt;
			Network::Timeout deadline;
			Network::TcpServer * server;
			Network::SocketSet in;
	};

} // Log
