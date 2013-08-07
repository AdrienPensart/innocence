#pragma once

#include <network/TcpServer.hpp>
#include <network/SelectSet.hpp>

#include "Observable.hpp"
#include "LogObserver.hpp"
#include "NonCopyable.hpp"
#include "Message.hpp"

namespace Common
{
	class LogServer : public Observable<LogObserver, Message>, public Common::NonCopyable
	{
		public:

			LogServer(const Network::Port& port);
			void run();
			void stop();

		private:

			bool interrupt;
			Network::Timeout deadline;
			Network::TcpServer * server;
			Network::SocketSet in;
	};

} // Common
