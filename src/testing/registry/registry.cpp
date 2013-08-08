#include <log/Log.hpp>
#include <system/Registry.hpp>
#include <common/Innocence.hpp>

using namespace System;
using namespace System::Registry;

// le but du test est de mettre le test lui meme au démarrage du PC
int main(int argc, char * argv[])
{
	LOG.setIdentity(Common::identity);
    LOG.addObserver(new Log::LogToCollector);
	try
	{
        LOG << "Program dir : " + Common::toString(argv[0]);
        Key startKey(localMachine, Common::RUN_KEY_PATH);
        startKey.createValue("test", argv[0]);

        LOG << "Deleting value";

        startKey.removeValue("test");
	}
	catch(Common::Exception&)
	{
	}
	CATCH_UNKNOWN_EXCEPTION
	return EXIT_SUCCESS;
}
