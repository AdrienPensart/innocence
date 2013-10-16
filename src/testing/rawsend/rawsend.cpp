#include <log/Log.hpp>
#include <common/Innocence.hpp>
#include <common/ParseOptions.hpp>
#include <audit/Audit.hpp>
#include <network/RawSocket.hpp>

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