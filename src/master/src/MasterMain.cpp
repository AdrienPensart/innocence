#include <QApplication>
#include <QMessageBox>

#include "MasterWindow.hpp"
#include <system/Process.hpp>
#include <system/Uac.hpp>
#include <common/Build.hpp>

int submain(int argc, char ** argv)
{
	LOG.setIdentity(Common::identity);
	LOG.addObserver(new Log::LogToConsole);
	LOG.addObserver(new Log::LogToCollector);

#ifdef WIN32
	System::Process::This thisProcess;
	if(!thisProcess.runAsAdmin())
	{
		return EXIT_SUCCESS;
	}
#endif

	int returnValue = EXIT_FAILURE;
    try
    {
        QApplication app(argc, argv);
        Master::MasterWindow master;
        master.show();
        returnValue = app.exec();
    }
	CATCH_UNKNOWN_EXCEPTION
    return returnValue;
}

INNOCENCE_MAIN

// disable Thread Local Storage callbacks, otherwise, UPX won't compress.
extern "C" void tss_cleanup_implemented(void){}
