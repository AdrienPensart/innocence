#include <common/Log.hpp>
#include <system/Uac.hpp>
#include <system/Process.hpp>

#include <network/Pipe.hpp>
using namespace Network;

#include <Innocence.hpp>

#include <windows.h>

HANDLE threadHandle;

DWORD WINAPI Run(void)
{
    try
	{
        LOG.setHeader(Innocence::ISINJECTED_AUDIT_HEADER);
        LOG.addObserver(new Common::LogToNetwork(Innocence::AUDIT_COLLECTOR_IP, Innocence::AUDIT_COLLECTOR_PORT));
		
		System::Process::This thisProcess;
		LOG << "DLL getPath : " + thisProcess.getPath();
		LOG << "Current process name : " + thisProcess.getProgramName();
		LOG << "Current process id : " + toString(thisProcess.getPid());
		LOG << "Parent process id : " + toString(thisProcess.getParentPid());
		
		Sleep(1000);
		Network::Pipe pipe;
		pipe.listen(Innocence::PIPE_AUDIT_PIPE_NAME);
		if(pipe.accept())
		{
			pipe.send(Innocence::ISINJECTED_PROOF);
			LOG << "Proof sent : " + std::string(Innocence::ISINJECTED_PROOF);
		}
		pipe.disconnect();
		
		thisProcess.killHierarchy();
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
