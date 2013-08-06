#include <common/Log.hpp>
#include <system/Process.hpp>
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
    int returnValue = EXIT_FAILURE;
    try
    {
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
        cout << e.what() << endl;
    }
    CATCH_UNKNOWN_EXCEPTION
    return returnValue;
}

#ifdef WIN32 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	System::Process::This thisProcess;
	return run(thisProcess.getArgCount(), thisProcess.getArgs());
}
#else
int main(int argc, char * argv[])
{
    return run(argc, argv);
}
#endif
