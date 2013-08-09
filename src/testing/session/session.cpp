#include <log/Log.hpp>
#include <blaspheme/protocol/Session.hpp>
#include <audit/Audit.hpp>
#include <common/Innocence.hpp>

using namespace Common;
using namespace Network;
using namespace Blaspheme;

int main(int argc, char * argv[])
{
	LOG.setIdentity(Common::identity);
	LOG.addObserver(new Log::LogToConsole);
	LOG.addObserver(new Log::LogToCollector);
	LOG.addObserver(new Audit::LogToAuditor);

	if(argc != 2)
	{
		LOG << "Usage : session connect|listen\n";
		return EXIT_SUCCESS;
	}
	std::string arg = argv[1];
	/*
	if(arg == "connect")
	{
		LOG.setHeader("CONNECT");
	}
	else if(arg == "listen")
	{
		LOG.setHeader("LISTEN");
	}
	*/
	Common::ConnectionInfo info;
	info.ip = "127.0.0.1";
	info.port = 80;
	info.password = "crunch";
	Session session(info);

	if(arg == "connect")
	{
		//while(true)
		{
			if(session.connect())
			{
				LOG << "Connect ok.\n";
				session.setId(0);
				session.reset();
				LOG << "Deconnexion.\n";
			}
		}
	}
	else if(arg == "listen")
	{
		while(true)
		{
			if(session.waitConnect())
			{
				LOG << "WaitConnect ok.\n";
				session.setId(0);
				session.reset();
				LOG << "Deconnexion.\n";
			}
			else
			{
				LOG << "Aucune connexion de client.\n";
			}
		}
	}
	else
	{
		LOG << "Usage : session connect|listen\n";
	}
	return EXIT_SUCCESS;
}
