#include <log/Log.hpp>
#include <system/Uac.hpp>
#include <common/ParseOptions.hpp>
#include <audit/Audit.hpp>

int submain(int argc, char ** argv) {
	try {
		LOG.setIdentity(Common::identity);
		Common::ParseOptions(argc, argv);

		if(System::isAdministrator()) {
			LOG << "You are administrator !";
			return EXIT_SUCCESS;
		}
		LOG << "You are NOT administrator";
	}
	CATCH_COMMON_EXCEPTION
	CATCH_UNKNOWN_EXCEPTION
	return EXIT_FAILURE;
}

INNOCENCE_MAIN
