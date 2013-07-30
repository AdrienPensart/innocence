#include <common/Log.hpp>
#include <blaspheme/protocol/Session.hpp>


#include <iostream>
using namespace std;
using namespace Common;
using namespace Network;
using namespace Blaspheme;


int main(int argc, char * argv[])
{
	LOG.setHeader("SESSION");
	LOG.addObserver(new LogToNetwork("127.0.0.1", 80));

	if(argc != 2)
	{
		cout << "Usage : session connect|listen\n";
		return EXIT_SUCCESS;
	}
	string arg = argv[1];
	if(arg == "connect")
	{
		LOG.setHeader("CONNECT");
	}
	else if(arg == "listen")
	{
		LOG.setHeader("LISTEN");
	}

	Session session;
	ConnectionInfo info;
	info.ip = "127.0.0.1";
	info.port = 80;
	info.deadline.set(60);
	info.password = "crunch";
	
	if(arg == "connect")
	{
		//while(true)
		{
			if(session.connect(info))
			{
				cout << "Connect ok.\n";
				session.setId(0);
				session.reset();
				cout << "Deconnexion.\n";
			}
		}
	}
	else if(arg == "listen")
	{
		while(true)
		{
			if(session.wait_connect(info))
			{
				cout << "WaitConnect ok.\n";
				session.setId(0);
				session.reset();
				cout << "Deconnexion.\n";
			}
			else
			{
				cout << "Aucune connexion de client.\n";
			}
		}
	}
	else
	{
		cout << "Usage : session connect|listen\n";
	}
	return EXIT_SUCCESS;
}
