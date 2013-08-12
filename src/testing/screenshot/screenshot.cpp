#include <log/Log.hpp>
#include <malicious/Screenshot.hpp>
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
		Malicious::Screenshot sc;
		sc.take("test.jpg", 100);
	}
	catch(Common::Exception&)
	{
	}
	CATCH_UNKNOWN_EXCEPTION
	return EXIT_SUCCESS;
}

INNOCENCE_MAIN
