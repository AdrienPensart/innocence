#include <common/Log.hpp>
#include <system/Registry.hpp>
#include <Innocence.hpp>

using namespace System;
using namespace System::Registry;

// le but du test est de mettre le test lui meme au démarrage du PC
int main(int argc, char * argv[])
{
	LOG.setIdentity(Innocence::identity);
    LOG.addObserver(new Common::LogToCollector);
	try
	{
        LOG << "Program dir : " + toString(argv[0]);
        Key startKey(localMachine, Innocence::RUN_KEY_PATH);
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
