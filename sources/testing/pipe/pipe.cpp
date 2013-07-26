#include <common/Log.hpp>
#include <network/Pipe.hpp>
using namespace Network;
using namespace std;

static const char * PIPE_NAME = "\\\\.\\pipe\\test_pipe";

int main(int argc, char * argv[])
{
	LOG.setHeader("TEST PIPE");
    LOG.addObserver(new Common::LogToNetwork("127.0.0.1", 80));
	LOG.addObserver(new Common::LogToConsole);

	try
	{
		if(argc != 2)
		{
			LOG << "Incorrect number of arguments.";
			return EXIT_SUCCESS;
		}
		string type = argv[1];
		if(type == "server")
		{
			LOG << "Server mode";
			string buffer = "un message !";
			Network::Pipe pipe_server;
			pipe_server.listen(PIPE_NAME);
			if(pipe_server.accept())
			{
				LOG << "Sending test : "+buffer;
				pipe_server.send(buffer);
			}
			pipe_server.disconnect();
		}
		else if(type == "client")
		{
			LOG << "Client mode";
			Network::Pipe pipe_client;
			if(pipe_client.connect(PIPE_NAME))
			{
				string buffer;
				pipe_client.recv(buffer);
				LOG << "Received message : " + buffer;
				pipe_client.disconnect();
			}
		}
		else
		{
			LOG << "Incorrect parameter";
		}
	}
	catch(exception& e)
	{
		LOG << e.what();
	}
	catch(...)
	{
		LOG << "Unknown exception";
	}
	return EXIT_SUCCESS;
}
