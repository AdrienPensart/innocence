#include <common/Log.hpp>
#include <system/ThisProcess.hpp>
#include <iostream>
#include <cstdlib>
using namespace std;

#include <QApplication>
#include <QMessageBox>
#include "DebugThread.hpp"
#include "DebuggingServerWindow.hpp"
using namespace Debugger;

int run(int argc, char ** argv)
{
    int returnValue = EXIT_SUCCESS;
    try
    {
        // Thread et Fenetre de debuggage
		QApplication app(argc, argv);
		DebugThread debug_thread(80);
		DebuggingServerWindow debug_dialog;
		QObject::connect(&debug_thread, SIGNAL(newDebugMsg(QString)), &debug_dialog, SLOT(addDebugMsg(QString)));
		debug_thread.start();
		debug_dialog.show();
		returnValue = app.exec();
    }
    catch(std::exception& e)
    {
        cout << "Exception standard : " << e.what() << '\n';
    }
    catch(...)
    {
		cout << "Probleme d'origine inconnue.\n";
        return EXIT_FAILURE;
    }
    return returnValue;
}

#ifdef WIN32 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	System::ThisProcess thisProcess;
	return run(thisProcess.getArgCount(), thisProcess.getArgs());
}
#else
int main(int argc, char * argv[])
{
    return run(argc, argv);
}
#endif
