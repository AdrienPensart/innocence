#include <log/Log.hpp>
#include <malicious/Screenshot.hpp>
#include <common/Innocence.hpp>
#include <audit/Audit.hpp>

int main(int argc, char * argv[])
{
	LOG.setIdentity(Common::identity);
	LOG.addObserver(new Log::LogToConsole);
	LOG.addObserver(new Log::LogToCollector);
	LOG.addObserver(new Audit::LogToAuditor);

	Malicious::Screenshot sc;
	sc.take("test.jpg", 100);
	return EXIT_SUCCESS;
}
