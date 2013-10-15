#include <log/Log.hpp>
#include <system/Registry.hpp>
#include <common/ParseOptions.hpp>
#include <common/Innocence.hpp>
#include <audit/Audit.hpp>

int submain(int argc, char ** argv)
{
	try
	{
		LOG.setIdentity(Common::identity);
		Common::ParseOptions(argc, argv);
        LOG << "Program dir : " + Common::toString(argv[0]);
		System::Registry::Key startKey(System::Registry::localMachine, Common::RUN_KEY_PATH);
        startKey.createValue("test", argv[0]);

        LOG << "Deleting value";

        startKey.removeValue("test");
	}
	CATCH_COMMON_EXCEPTION
	CATCH_UNKNOWN_EXCEPTION
	return EXIT_SUCCESS;
}

INNOCENCE_MAIN
