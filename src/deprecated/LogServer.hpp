#ifndef _LOG_SERVER_
#define _LOG_SERVER_

#include <windows.h>
#include <string>
#include <network/UdpSocket.hpp>

namespace Common {
	class LogServer {
		public:

			LogServer(Network::Port port, const std::string& onProof);
			~LogServer();
			void start();
			void stop();
			const std::string& getProof();
			bool isProofReceived();
			Network::Port getPort();

		private:

			static DWORD WINAPI MsgLoop(LPVOID lpParameter);

			Network::Port port;
			std::string onProof;
			bool proofReceived;
			bool interrupted;
			DWORD dwThread;
			HANDLE hThread;
	};
} // Common

#endif // _LOG_SERVER_
