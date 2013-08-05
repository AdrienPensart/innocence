#include <system/Uac.hpp>
#include <system/Process.hpp>

#include <network/Network.hpp>
using namespace Network;

#include <common/Log.hpp>

void audit(const std::string& auditExe)
{
	System::Process::Launcher auditExeProcess(auditExe);
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
	
	System::Process::This thisProcess;
    if(!System::isAdministrator())
	{
        LOG << "Trying to run as administrator";
	    System::RunAsAdministrator(thisProcess.getProgramName(), thisProcess.getProgramDir());
		return EXIT_SUCCESS;
	}
	
	audit("elevator.exe");
	audit("injection.exe");
	audit("hideproc.exe");
	return EXIT_SUCCESS;
}
