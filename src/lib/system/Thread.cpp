#include "Thread.hpp"

#include <log/Log.hpp>

namespace System
{
	Thread::Thread(LPTHREAD_START_ROUTINE routineArg, LPVOID parameterArg) : 
		routine(routineArg),
		parameter(parameterArg),
		dwThread(0),
		hThread(0),
		running(0)
	{
	}
	
	Thread::~Thread()
	{
	}

	bool Thread::isRunning()
	{
		return running;
	}

	bool Thread::join(const Network::Timeout& deadline)
	{
		if(running)
		{
			if(WaitForSingleObject(hThread, deadline.getMs()) == WAIT_OBJECT_0)
			{
				running = false;
			}
		}
		return !running;
	}

	void Thread::start()
	{
		TRACE_FUNCTION
        if(!running)
        {
            hThread = CreateThread(0, 0, routine, parameter, 0, &dwThread);
		    if(hThread == NULL)
		    {
				throw Common::Exception("CreateThreadfailed : " + Common::toString(GetLastError()));
		    }
            running = true;
        }
	}

	void Thread::stop()
	{
		TRACE_FUNCTION
        if(running)
        {
            if(!TerminateThread(hThread, 0))
		    {
				throw Common::Exception("TerminateThread failed : " + Common::toString(GetLastError()));
		    }
            running = false;
        }
	}

} // System
