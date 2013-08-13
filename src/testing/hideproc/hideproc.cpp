#include <malicious/ProcessHider.hpp>
#include <system/Uac.hpp>
#include <system/Process.hpp>
#include <log/Log.hpp>
#include <audit/Audit.hpp>
#include <common/ParseOptions.hpp>

int submain(int argc, char ** argv)
{
	int exitCode = EXIT_FAILURE;
    try
	{
        LOG.setIdentity(Common::identity);
		Common::ParseOptions(argc, argv);

		System::Process::This thisProcess;
		if(!thisProcess.runAsAdmin())
		{
			return EXIT_SUCCESS;
		}

        LOG << "Hiding the process";
        Malicious::ProcessHider hider;
		hider.hide(thisProcess.getProgramName());

		System::Process::Map pm;
		System::Process::GetProcessList(pm);

		for(System::Process::Map::iterator i = pm.begin(); i != pm.end(); i++)
		{
			if(i->first ==  thisProcess.getPid())
			{
				exitCode = EXIT_SUCCESS;
				break;
			}
		}
        hider.remove();
	}
    catch(Malicious::DriverError& e)
    {
		LOG << e.what();
    }
	CATCH_UNKNOWN_EXCEPTION
	return exitCode;
}

INNOCENCE_MAIN
