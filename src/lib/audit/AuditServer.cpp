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

	AuditServer::AuditServer() : 
		Thread((LPTHREAD_START_ROUTINE)LogLoop)
	{
	}

	void AuditServer::setLogServer(Log::LogServer * logServerArg)
	{
		logServer = logServerArg;
	}

	void AuditServer::start()
	{
		TRACE_FUNCTION
		if(logServer)
		{
			Thread::start();
			LOG << "AuditServer started";
		}
	}

    void AuditServer::stop()
	{
		TRACE_FUNCTION
		if(logServer)
		{
			logServer->stop();
			if(join())
			{
				LOG << "AuditServer stopped";
			}
		}
	}

	DWORD WINAPI AuditServer::LogLoop(LPVOID lpParameter)
	{
		TRACE_FUNCTION
		if(logServer)
		{
			logServer->run();
		}
		return 0;
	}
} // Audit
