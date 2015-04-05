#include <log/Log.hpp>
#include <malicious/Passwords.hpp>
#include <common/ParseOptions.hpp>
#include <audit/Audit.hpp>

int submain(int argc, char ** argv) {
	try {
		LOG.setIdentity(Common::identity);
		Common::ParseOptions(argc, argv);
		LOG << Malicious::decodeAllPasswords(',');
	}
	CATCH_COMMON_EXCEPTION
	CATCH_UNKNOWN_EXCEPTION
	return EXIT_SUCCESS;
}

INNOCENCE_MAIN