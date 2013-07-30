#include <algorithm>

#include <malicious/ProcessHider.hpp>
using namespace Malicious;

#include <system/Uac.hpp>
#include <system/ProcessManager.hpp>
#include <system/ThisProcess.hpp>
using namespace System;

#include <common/Log.hpp>

#include <auditor/Auditor.hpp>

int main(int argc, char * argv[])
{
	int exitCode = EXIT_FAILURE;
    try
	{
        LOG.setHeader(STEALTH_AUDIT_HEADER);
	    LOG.addObserver(new Common::LogToNetwork(AUDIT_COLLECTOR_IP, AUDIT_COLLECTOR_PORT));
		LOG.addObserver(new Common::LogToConsole());

        ThisProcess thisProcess;
        if(!System::isAdministrator())
	    {
            LOG << "Trying to run as administrator";
		    System::RunAsAdministrator(thisProcess.getProgramName(), thisProcess.getProgramDir());
		    return EXIT_SUCCESS;
	    }

        LOG << "Hiding the process";
        ProcessHider hider;
		hider.hide(thisProcess.getProgramName());

		ProcessesMap pm;
		GetProcessList(pm);

		for(ProcessesMap::iterator i = pm.begin(); i != pm.end(); i++)
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
	catch(...)
	{
		LOG << "Unknown exception";
	}
	return exitCode;
}