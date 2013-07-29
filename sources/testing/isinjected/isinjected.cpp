#include <common/Log.hpp>
#include <system/Uac.hpp>
using namespace System;

#include <network/Pipe.hpp>
using namespace Network;

#include <auditor/Auditor.hpp>

#include <windows.h>

HANDLE threadHandle;

DWORD WINAPI Run(void)
{
    try
	{
        LOG.setHeader(ISINJECTED_AUDIT_HEADER);
        LOG.addObserver(new Common::LogToNetwork(AUDIT_COLLECTOR_IP, AUDIT_COLLECTOR_PORT));
		
		Sleep(1000);
		Network::Pipe pipe;
		pipe.listen(PIPE_AUDIT_PIPE_NAME);
		if(pipe.accept())
		{
			pipe.send(ISINJECTED_PROOF);
			LOG << "Proof sent : " + std::string(ISINJECTED_PROOF);
		}
		pipe.disconnect();
    }
    catch(std::exception& e)
    {
        LOG << e.what();
    }
    catch(...)
    {
        LOG << "Unknow exception from DLL";
    }
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
