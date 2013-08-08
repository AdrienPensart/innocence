#include <system/Uac.hpp>
#include <system/Process.hpp>

#include <network/Network.hpp>
using namespace Network;

#include <common/Build.hpp>
#include <log/Log.hpp>
#include <audit/Audit.hpp>

void audit(const std::string& auditExe)
{
	Audit::Run run(Common::identity.getModule(), Common::identity.getBuildId(), Common::identity.getBuildTimestamp());

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
	LOG.setIdentity(Common::identity);
	LOG.addObserver(new Log::LogToCollector);
	LOG.addObserver(new Log::LogToConsole);
	
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
