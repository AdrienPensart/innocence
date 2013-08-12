#include "AuditServer.hpp"
#include <log/Log.hpp>

namespace Audit
{
	DispatchAudit::DispatchAudit(GlobalAudit& globalAuditArg) : 
		globalAudit(globalAuditArg)
	{
	}
    
	void DispatchAudit::update(const Log::Message& message)
	{
		globalAudit.addMessage(message);
	}

	Log::LogServer * AuditServer::logServer = 0;

	void AuditServer::setLogServer(Log::LogServer * logServerArg)
	{
		logServer = logServerArg;
	}

	void AuditServer::start()
	{
		TRACE_FUNCTION
		if(logServer)
		{
			Thread::start((LPTHREAD_START_ROUTINE)LogLoop);
			LOG << "AuditServer started";
		}
	}

    void AuditServer::stop()
	{
		TRACE_FUNCTION
        if(isRunning())
        {
			AuditServer::instance().logServer->stop();
            join();
			LOG << "AuditServer stopped";
		}
	}

	DWORD WINAPI AuditServer::LogLoop(LPVOID lpParameter)
	{
		TRACE_FUNCTION
		logServer->run();
		return 0;
	}
} // Audit
