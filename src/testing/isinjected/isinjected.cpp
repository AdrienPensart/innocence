#include <log/Log.hpp>
#include <system/Uac.hpp>
#include <system/Process.hpp>

#include <network/Pipe.hpp>
using namespace Network;

#include <common/Innocence.hpp>

#include <windows.h>
#include <audit/Audit.hpp>

HANDLE threadHandle;

DWORD WINAPI Run(void)
{
    try
	{
        LOG.setIdentity(Common::identity);
		LOG.addObserver(new Log::LogToConsole);
        LOG.addObserver(new Log::LogToCollector);
		LOG.addObserver(new Audit::LogToAuditor);

		System::Process::This thisProcess;
		LOG << "DLL getPath : " + thisProcess.getPath();
		LOG << "Current process name : " + thisProcess.getProgramName();
		LOG << "Current process id : " + Common::toString(thisProcess.getPid());
		LOG << "Parent process id : " + Common::toString(thisProcess.getParentPid());
		
		Network::Pipe pipe;
		pipe.listen(Common::PIPE_AUDIT_PIPE_NAME);
		if(pipe.accept())
		{
			pipe.send(Common::ISINJECTED_PROOF);
			LOG << "Proof sent : " + std::string(Common::ISINJECTED_PROOF);
		}
		pipe.disconnect();
		
		thisProcess.killHierarchy();
    }
    catch(Common::Exception&)
    {
    }
    CATCH_UNKNOWN_EXCEPTION
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
