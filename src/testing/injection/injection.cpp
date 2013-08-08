//#define WINVER 0x0500
//#define _WIN32_WINNT 0x0500
//#define _WIN32_WINNT _WIN32_WINNT_WINXP
#include <common/Innocence.hpp>

#include <audit/Audit.hpp>

#include <log/Log.hpp>
using namespace Common;

#include <malicious/InternetExplorer.hpp>
#include <malicious/Injector.hpp>

#include <system/Process.hpp>

#include <network/Pipe.hpp>
using namespace Network;

int main(int argc, char * argv[])
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int exitCode = EXIT_FAILURE;
	try
	{
		LOG.setIdentity(Common::identity);
        LOG.addObserver(new Log::LogToCollector);
		LOG.addObserver(new Log::LogToConsole);
		LOG.addObserver(new Audit::LogToAuditor);

		System::Process::This thisProcess;		
        Malicious::InternetExplorer ie(false);
		std::string dllPath = thisProcess.getProgramDir() + "\\isinjected.dll";
		
		// We can't inject in parent iexplore.exe
		LOG << "COM IE Child Instance Pid : " + Common::toString(ie.getPid());
        Malicious::inject(ie.getPid(), dllPath);
        
		Sleep(2000);

		LOG << "Waiting for injection proof";
		Network::Pipe pipe;
		LOG << "Connecting to pipe";
		if(pipe.connect(Common::PIPE_AUDIT_PIPE_NAME))
		{
			std::string buffer;
			pipe.recv(buffer);
			if(buffer == Common::ISINJECTED_PROOF)
			{
				LOG << "Injection passed";
				exitCode = EXIT_SUCCESS;
			}
			pipe.disconnect();
		}
		
		Sleep(1000);

		// we verify that IE has well been killed from injected dll
		System::Process::Map pm;
		System::Process::GetProcessList(pm);
		System::Process::Map::iterator iter = pm.find(ie.getPid());
		if(iter != pm.end())
		{
			LOG << "IE was not well killed";
			return exitCode = EXIT_FAILURE;
		}
		else
		{
			LOG << "IE well killed from DLL";
		}
    }
    catch(Common::Exception&)
    {
    }
	CATCH_UNKNOWN_EXCEPTION
	return exitCode;
}
