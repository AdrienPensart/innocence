#include <common/Log.hpp>
#include <system/Registry.hpp>
#include <auditor/Auditor.hpp>

using namespace System;
using namespace System::Registry;
using namespace std;

// le but du test est de mettre le test lui meme au démarrage du PC
int main(int argc, char * argv[])
{
    static const char * RUN_KEY = "Software\\Microsoft\\Windows\\CurrentVersion\\Run\\";
	LOG.setHeader(REGISTRE_AUDIT_HEADER);
    LOG.addObserver(new Common::LogToNetwork(AUDIT_COLLECTOR_IP, AUDIT_COLLECTOR_PORT));
	try
	{
        LOG << "Program dir : " + toString(argv[0]);
        Key startKey(localMachine, RUN_KEY);
        startKey.createValue("test", argv[0]);

        LOG << "Deleting value";

        startKey.removeValue("test");
	}
	catch(RegistryBaseError&)
	{
		LOG << "Bad registry base";
	}
	catch(KeyDoesntExist&)
	{
		LOG << "Key does not exist";
	}
	catch(KeyErrorCreate&)
	{
		LOG << "Error while creating key";
	}
	catch(RegistryError&)
	{
		LOG << "Registry error";
	}
	catch(...)
	{
		LOG << "Unknown exception";
	}
	return EXIT_SUCCESS;
}
