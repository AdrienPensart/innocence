#include <system/Uac.hpp>
#include <system/Process.hpp>
#include <system/File.hpp>
#include <network/Network.hpp>
using namespace Network;

#include <common/Innocence.hpp>
#include <log/Log.hpp>
#include <log/LogServer.hpp>

#include <audit/GlobalAudit.hpp>
#include <audit/Run.hpp>
#include <audit/AuditServer.hpp>

int main(int argc, char argv[])
{
	LOG.setIdentity(Common::identity);
	//LOG.addObserver(new Log::LogToConsole);
	//LOG.addObserver(new Log::LogToCollector);
	
	System::Process::This thisProcess;
    if(!System::isAdministrator())
	{
        LOG << "Trying to run as administrator";
	    System::RunAsAdministrator(thisProcess.getProgramName(), thisProcess.getProgramDir());
		return EXIT_SUCCESS;
	}
	
	Audit::GlobalAudit globalAudit;
	Log::LogServer logServer(Common::AUDIT_SERVER_PORT);
	//logServer.addObserver(new Log::LogToConsole);
	//logServer.addObserver(new Audit::DispatchAudit(globalAudit));

	Audit::AuditServer::instance().setLogServer(&logServer);
	Audit::AuditServer::instance().start();

	//globalAudit.addRun(new Audit::Run("elevator.exe"));
	globalAudit.addRun(new Audit::Run("injection.exe"));
	//globalAudit.addRun(new Audit::Run("hideproc.exe"));
	globalAudit.run();

	Audit::AuditServer::instance().stop();

	globalAudit.build();

	//system("pause");
	return EXIT_SUCCESS;
}
