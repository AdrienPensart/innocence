#include "DebugThread.hpp"
#include <iostream>

namespace Debugger
{
	DebugThread::DebugThread(Network::Port default_port)
	:terminating(false)
	{
		server.listen(default_port);
	}

	DebugThread::~DebugThread()
	{
		terminating = true;
		wait();
	}

	void DebugThread::run()
	{
		try
		{
			Network::Timeout deadline(0, 100);
			std::string buffer;
			while(!terminating)
			{
				server.recv(buffer, deadline);
				if(buffer.size())
				{
					std::cout << buffer;
					QString q_buffer(buffer.c_str());
					emit newDebugMsg(q_buffer);
				}
			}
		}
		catch(...)
		{
		}
	}
} // Debugger
