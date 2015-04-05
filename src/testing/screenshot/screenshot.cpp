#include <log/Log.hpp>
#include <malicious/Screenshot.hpp>
#include <common/ParseOptions.hpp>
#include <common/Innocence.hpp>
#include <audit/Audit.hpp>

int submain(int argc, char ** argv) {
	try {
		LOG.setIdentity(Common::identity);
		Common::ParseOptions(argc, argv);
		Malicious::Screenshot sc;
		sc.take("test.jpg", 100);
	}
	CATCH_COMMON_EXCEPTION
	CATCH_UNKNOWN_EXCEPTION
	return EXIT_SUCCESS;
}

INNOCENCE_MAIN
