#pragma once

#include "Socket.hpp"

namespace Network
{
	class TcpSocket : public Socket
	{
		protected:
			virtual void acquire();
	};

} // Network
