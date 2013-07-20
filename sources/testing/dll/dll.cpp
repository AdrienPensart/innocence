#include <windows.h>
#include <common/Log.hpp>
#include <system/Uac.hpp>
using namespace System;

HANDLE threadHandle;

DWORD WINAPI Run(void)
{
    try
	{
        LOG.setHeader("TEST DLL Injection + Elevation");
        LOG.addObserver(new Common::LogToNetwork("127.0.0.1", 80));
        LOG << "Injection DLL : OK\n";
        LOG << "Elevation : " + GetElevationType();
    }
    catch(std::exception& e)
    {
        LOG << std::string("Exception standard : ") + e.what();
    }
    catch(...)
    {
        LOG << "Erreur d'origine inconnue.";
    }
    // si une erreur devait survenir, il faut s'assurer que le processus injecté meurre avec la DLL
    exit(0);
	return EXIT_SUCCESS;
}

extern "C" BOOL APIENTRY DllMain (HINSTANCE hModule, DWORD dwMsg, LPVOID lpReserved)
{
    UNREFERENCED_PARAMETER( hModule );
    UNREFERENCED_PARAMETER( lpReserved );

    switch( dwMsg )
    {
		case DLL_PROCESS_ATTACH:
			DisableThreadLibraryCalls( hModule );
			threadHandle = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)Run, NULL, 0, 0 );
            return (!threadHandle) ? FALSE : TRUE;
		case DLL_PROCESS_DETACH:
			TerminateThread(threadHandle, EXIT_SUCCESS);
			return TRUE;
    }
    return TRUE;
}
