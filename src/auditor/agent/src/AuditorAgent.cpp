#include <system/Uac.hpp>
#include <system/Process.hpp>
#include <system/File.hpp>
#include <network/Network.hpp>

#include <common/ParseOptions.hpp>
#include <log/Log.hpp>
#include <log/LogServer.hpp>

#include <audit/GlobalAudit.hpp>
#include <audit/Run.hpp>
#include <audit/AuditServer.hpp>

int submain(int argc, char ** argv)
{
	try
	{
		LOG.setIdentity(Common::identity);
		Common::ParseOptions(argc, argv);
	
		System::Process::This thisProcess;
		if(!System::isAdministrator())
		{
			LOG << "Trying to run as administrator";
			System::RunAsAdministrator(thisProcess.getProgramName(), thisProcess.getProgramDir());
			return EXIT_SUCCESS;
		}
	
		Log::LogServer logServer(Common::AUDIT_SERVER_PORT);
		logServer.addObserver(new Log::LogToConsole);
		
		Audit::GlobalAudit globalAudit;
		logServer.addObserver(new Audit::DispatchAudit(globalAudit));
	
		Audit::AuditServer::instance().setLogServer(&logServer);
		Audit::AuditServer::instance().start();

		
		//globalAudit.addRun(new Audit::Run("elevator.exe"));
		globalAudit.addRun(new Audit::Run("injection.exe"));
		//globalAudit.addRun(new Audit::Run("hideproc.exe"));
		globalAudit.run();
		Audit::AuditServer::instance().stop();

		globalAudit.build();

		//system("pause");
	}
	CATCH_COMMON_EXCEPTION
	CATCH_UNKNOWN_EXCEPTION
	return EXIT_SUCCESS;
}

INNOCENCE_MAIN
