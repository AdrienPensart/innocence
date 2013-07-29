#include <common/Log.hpp>
#include <system/Process.hpp>
using namespace System;

#include <network/Network.hpp>
using namespace Network;

void audit(const std::string& auditExe)
{
	Process auditExeProcess(auditExe);
	DWORD auditResult = auditExeProcess.wait();

	if(auditResult == EXIT_SUCCESS)
	{
		LOG << "Test of " + auditExe + " passed => system vulnerable";
	}
	else
	{
		LOG << "Test of " + auditExe + " failed => system immuned";
	}
}

int main(int argc, char argv[])
{
	LOG.setHeader("AUDITOR");
	LOG.addObserver(new Common::LogToNetwork("127.0.0.1", 80));
	LOG.addObserver(new Common::LogToConsole);

	audit("elevator.exe");
	audit("injection.exe");
	return EXIT_SUCCESS;
}
