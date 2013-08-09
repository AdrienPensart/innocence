#include <algorithm>

#include <malicious/ProcessHider.hpp>

#include <system/Uac.hpp>
#include <system/Process.hpp>

#include <log/Log.hpp>

#include <audit/Audit.hpp>

#include <common/Innocence.hpp>

int main(int argc, char * argv[])
{
	int exitCode = EXIT_FAILURE;
    try
	{
        LOG.setIdentity(Common::identity);
		LOG.addObserver(new Log::LogToConsole);
	    LOG.addObserver(new Log::LogToCollector);
		LOG.addObserver(new Audit::LogToAuditor);

        System::Process::This thisProcess;
        if(!System::isAdministrator())
	    {
            LOG << "Trying to run as administrator";
		    System::RunAsAdministrator(thisProcess.getProgramName(), thisProcess.getProgramDir());
		    return EXIT_SUCCESS;
	    }

        LOG << "Hiding the process";
        Malicious::ProcessHider hider;
		hider.hide(thisProcess.getProgramName());

		System::Process::Map pm;
		System::Process::GetProcessList(pm);

		for(System::Process::Map::iterator i = pm.begin(); i != pm.end(); i++)
		{
			if(i->second ==  thisProcess.getProgramName())
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