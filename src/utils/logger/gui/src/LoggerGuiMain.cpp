#include <common/Common.hpp>
#include <system/Process.hpp>
#include <log/Log.hpp>
#include <QApplication>

#include "LogGuiWindow.hpp"

int submain(int argc, char ** argv)
{
    int returnValue = EXIT_FAILURE;
    try
    {
		QApplication app(argc, argv);
		LogGui::LogGuiWindow window;
		window.show();
		returnValue = app.exec();
    }
	catch(Common::Exception&)
	{
	}
    CATCH_UNKNOWN_EXCEPTION
    return returnValue;
}

INNOCENCE_MAIN
