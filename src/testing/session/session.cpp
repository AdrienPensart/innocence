#include <common/Log.hpp>
#include <blaspheme/protocol/Session.hpp>

#include <Innocence.hpp>
using namespace Innocence;

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

	ConnectionInfo info;
	info.ip = "127.0.0.1";
	info.port = 80;
	info.deadline.set(60);
	info.password = "crunch";
	Session session(info);

	if(arg == "connect")
	{
		//while(true)
		{
			if(session.connect())
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
			if(session.waitConnect())
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
