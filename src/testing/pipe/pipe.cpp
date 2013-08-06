#include <common/Log.hpp>
#include <network/Pipe.hpp>
using namespace Network;
using namespace std;

#include <Innocence.hpp>

int main(int argc, char * argv[])
{
	LOG.setHeader(Innocence::PIPE_AUDIT_HEADER);
    LOG.addObserver(new Common::LogToCollector);
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
			pipe_server.listen(Innocence::PIPE_AUDIT_PIPE_NAME);
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
			if(pipe_client.connect(Innocence::PIPE_AUDIT_PIPE_NAME))
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
	catch(Common::Exception&)
	{
	}
	CATCH_UNKNOWN_EXCEPTION
	return EXIT_SUCCESS;
}
