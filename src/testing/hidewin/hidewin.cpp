#include <malicious/InternetExplorer.hpp>
#include <log/Log.hpp>

#include <common/ParseOptions.hpp>
#include <audit/Audit.hpp>

int submain(int argc, char ** argv)
{
	int exitCode = EXIT_FAILURE;
    try
	{
        LOG.setIdentity(Common::identity);
		Common::ParseOptions(argc, argv);
		Malicious::InternetExplorer ie;
		LOG << "Internet Explorer PID : " + Common::toString(ie.getPid());
		Sleep(1000);
		ie.kill();
	}
	CATCH_UNKNOWN_EXCEPTION
	return exitCode;
}

INNOCENCE_MAIN
