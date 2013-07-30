#include <common/Log.hpp>
using namespace Common;

#include <malicious/InternetExplorer.hpp>
#include <malicious/Injector.hpp>
#include <malicious/Exception.hpp>

#include <system/ThisProcess.hpp>
using namespace System;

#include <auditor/Auditor.hpp>

#include <network/Pipe.hpp>
using namespace Network;

int main(int argc, char * argv[])
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int exitCode = EXIT_FAILURE;
	try
	{
		LOG.setHeader("AUDIT INJECTION");
		LOG.trace();
        LOG.addObserver(new Common::LogToNetwork("127.0.0.1", 80));
		LOG.addObserver(new Common::LogToConsole);

		ThisProcess thisProcess;

		
		// Chargement normal de la DLL
		//HMODULE hModule = LoadLibrary("isinjected.dll");
		//Sleep(INFINITE);
		
		
        Malicious::InternetExplorer ie(true);
		std::string dllPath = thisProcess.getProgramDir() + "\\isinjected.dll";
		LOG << "Injecting " + dllPath;
        if(!Malicious::inject(ie.getPid(), dllPath))
        {
			LOG << "Injection failed";
			return EXIT_FAILURE;
		}
		
		Sleep(2000);

		LOG << "Waiting for injection proof";
		Network::Pipe pipe;
		LOG << "Connecting to pipe";
		if(pipe.connect(PIPE_AUDIT_PIPE_NAME))
		{
			std::string buffer;
			pipe.recv(buffer);
			if(buffer == ISINJECTED_PROOF)
			{
				LOG << "Injection passed";
				exitCode = EXIT_SUCCESS;
			}
			pipe.disconnect();
		}
    }
    catch(std::exception& e)
    {
        LOG << e.what();
    }
	catch(...)
	{
		LOG << "Unknow exception from injection main";
	}
	return exitCode;
}
