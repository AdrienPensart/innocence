#include "Thread.hpp"

#include <log/Log.hpp>

namespace System
{
	bool Thread::isRunning()
	{
		return running;
	}

	void Thread::start(LPTHREAD_START_ROUTINE routine)
	{
		TRACE_FUNCTION
        // on ne démarre le keylogger uniquement s'il n'est pas déjà actif
        if(!running)
        {
            hThread = CreateThread(0, 0, routine, 0, 0, &dwThread);
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
