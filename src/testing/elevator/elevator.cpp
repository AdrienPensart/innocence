#include <log/Log.hpp>
using namespace Common;

#include <system/Process.hpp>
#include <common/Innocence.hpp>
#include <common/ParseOptions.hpp>
#include <malicious/Elevator.hpp>

#include <audit/Audit.hpp>

int submain(int argc, char **argv) {
	try {
		LOG.setIdentity(Common::identity);
		Common::ParseOptions(argc, argv);

		System::Process::This thisProcess;
		return Malicious::elevate(thisProcess.getProgramDir()+"\\isadmin.exe");
	}
	CATCH_COMMON_EXCEPTION
	CATCH_UNKNOWN_EXCEPTION
	return EXIT_FAILURE;
}

INNOCENCE_MAIN
