#include <log/Log.hpp>
#include <system/Registry.hpp>
#include <common/Innocence.hpp>
#include <audit/Audit.hpp>

// le but du test est de mettre le test lui meme au démarrage du PC
int main(int argc, char * argv[])
{
	LOG.setIdentity(Common::identity);
    LOG.addObserver(new Log::LogToConsole);
	LOG.addObserver(new Log::LogToCollector);
	LOG.addObserver(new Audit::LogToAuditor);
	try
	{
        LOG << "Program dir : " + Common::toString(argv[0]);
		System::Registry::Key startKey(System::Registry::localMachine, Common::RUN_KEY_PATH);
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
