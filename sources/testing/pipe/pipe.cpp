#include <common/Logger.hpp>
#include <network/Pipe.hpp>
using namespace Network;
using namespace std;

static const char * PIPE_NAME = "\\\\.\\pipe\\test_pipe";

int main(int argc, char * argv[])
{
	LOG.setHeader("TEST PIPE");
    LOG.addObserver(new Common::LoggingNetwork("127.0.0.1", 80));
	try
	{
		if(argc != 2)
		{
			LOG << "Nombre d'arguments incorrect.";
			return EXIT_SUCCESS;
		}
		string type = argv[1];
		if(type == "server")
		{
			LOG << "Mode serveur.";
			string buffer = "un message !";
			Network::Pipe pipe_server;
			pipe_server.listen(PIPE_NAME);
			if(pipe_server.accept())
			{
				LOG << "Test d'envoi : ";
				pipe_server.send(buffer);
			}
			pipe_server.disconnect();
		}
		else if(type == "client")
		{
			LOG << "Mode client.";
			Network::Pipe pipe_client;
			if(pipe_client.connect(PIPE_NAME))
			{
				string buffer;
				pipe_client.recv(buffer);
				LOG << "Message recu : " + buffer;
				pipe_client.disconnect();
			}
		}
		else
		{
			LOG << "Parametre incorrect.";
		}
	}
	catch(exception& e)
	{
		LOG << e.what();
	}
	catch(...)
	{
		LOG << "Erreur d'origine inconnue.";
	}
	return EXIT_SUCCESS;
}