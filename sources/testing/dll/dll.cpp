#include <windows.h>
#include <common/Log.hpp>
#include <system/Uac.hpp>
using namespace System;

HANDLE threadHandle;

DWORD WINAPI Run(void)
{
    try
	{
        LOG.setHeader("FROM TEST DLL");
		LOG.trace();
        LOG.addObserver(new Common::LogToNetwork("127.0.0.1", 8000));
		LOG.addObserver(new Common::LogToConsole);
		LOG.sendRaw("INJECTED");
    }
    catch(std::exception& e)
    {
        LOG << e.what();
    }
    catch(...)
    {
        LOG << "Unknow exception";
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
