#include <Build.hpp>

#include <system/Uac.hpp>
#include <system/Process.hpp>

#include <network/Network.hpp>
using namespace Network;

#include <common/Log.hpp>
#include <audit/Audit.hpp>

void audit(const std::string& auditExe)
{
	Audit::Run run(Innocence::identity.getModule(), Innocence::identity.getBuildId(), Innocence::identity.getBuildTimestamp());

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
	LOG.setIdentity(Innocence::identity);
	LOG.addObserver(new Common::LogToCollector);
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
