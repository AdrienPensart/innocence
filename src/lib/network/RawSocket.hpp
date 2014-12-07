#pragma once
#ifdef WIN32

#include "Socket.hpp"
#include "Packet.hpp"

namespace Network
{
	class RawSocket : public Socket
	{
		public:

			RawSocket(int protocol);
			int send(const char * buffer, size_t length);
			int send(Packet& packet);
			int recv(char * object, int sizeOfObject);
			int recv(Packet& packet);

			void listen(const Port& argPort);

		protected:
			int protocol;
			//AddrIn sAddr;
			virtual void acquire();
	};
} // Network

#endif