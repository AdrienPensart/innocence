#include <system/Process.hpp>
#include <common/Log.hpp>
#include <QApplication>

#include "LogGuiWindow.hpp"

int run(int argc, char ** argv)
{
    int returnValue = EXIT_FAILURE;
    try
    {
		QApplication app(argc, argv);
		LogGuiWindow window;
		window.show();
		returnValue = app.exec();
    }
    CATCH_UNKNOWN_EXCEPTION
    return returnValue;
}

#ifdef INNOCENCE_DEBUG
int main(int argc, char * argv[])
{
    return run(argc, argv);
}
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	System::Process::This thisProcess;
	return run(thisProcess.getArgCount(), thisProcess.getArgs());
}
#endif // INNOCENCE_DEBUG
