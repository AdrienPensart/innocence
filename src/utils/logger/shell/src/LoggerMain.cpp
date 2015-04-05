#include <log/LogServer.hpp>
#include <common/ParseOptions.hpp>
#include <common/Settings.hpp>
#include <common/Common.hpp>

int submain(int argc, char ** argv) {
	try {
		LOG.setIdentity(Common::identity);
		Common::ParseOptions(argc, argv);

		Log::LogServer logserver(Common::LOG_COLLECTOR_PORT);
		logserver.addObserver(new Log::LogToConsole);
		logserver.run();
	}
	CATCH_COMMON_EXCEPTION
	CATCH_UNKNOWN_EXCEPTION
	return EXIT_SUCCESS;
}

INNOCENCE_MAIN
