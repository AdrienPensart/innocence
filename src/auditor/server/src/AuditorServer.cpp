#include <common/ParseOptions.hpp>

int submain(int argc, char ** argv)
{
	try
	{
		LOG.setIdentity(Common::identity);
		Common::ParseOptions(argc, argv);
	}
	CATCH_COMMON_EXCEPTION
	CATCH_UNKNOWN_EXCEPTION
	return EXIT_SUCCESS;
}

INNOCENCE_MAIN
