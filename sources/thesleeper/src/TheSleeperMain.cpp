//!
//!     Kaleidoscalp, all rights reserved.
//!

#include <QApplication>
#include <QMessageBox>
#include <iostream>
#include <system/ThisProcess.hpp>
#include <system/Uac.hpp>
#include "ServerWindow.hpp"

int run(int argc, char ** argv)
{
    int returnValue;
    try
    {
        QApplication app(argc, argv);
        TheSleeper::ServerWindow server;
        server.show();
        returnValue = app.exec();
    }
    catch(...)
    {
        std::cout << "Erreur d'origine inconnue.\n";
        return EXIT_FAILURE;
    }
    return returnValue;
}

//! if only Windows was an UNIX system...
#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    LOG.setHeader("MASTER");
	LOG.addObserver(new Common::LoggingNetwork("127.0.0.1", 80));

    // administrator rights are preferred to execute the Server
    System::ThisProcess thisProcess;
    if(!System::isAdministrator())
	{
        LOG << "Try to run as administrator.";
		System::RunAsAdministrator(thisProcess.getProgramName(), thisProcess.getProgramDir(), true);
		return EXIT_SUCCESS;
	}
	return run(thisProcess.getArgCount(), thisProcess.getArgs());
}
#else
int main(int argc, char * argv[])
{
    return run(argc, argv);
}
#endif // _WIN32

//! disable Thread Local Storage callbacks, otherwise, UPX won't compress.
extern "C" void tss_cleanup_implemented(void){}
