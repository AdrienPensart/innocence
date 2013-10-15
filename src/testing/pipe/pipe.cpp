#include <network/Pipe.hpp>
#include <system/Thread.hpp>
#include <common/ParseOptions.hpp>

class PipeServerThread : public System::Thread
{
	public:
		virtual void start()
		{
			Thread::start((LPTHREAD_START_ROUTINE)PipeServerLoop);
		}

	private:
		static DWORD WINAPI PipeServerLoop(LPVOID lpParameter)
		{

		}
};

int submain(int argc, char ** argv)
{
	try
	{
		LOG.setIdentity(Common::identity);
		Common::ParseOptions(argc, argv);
		

		std::string type = argv[1];
		if(type == "server")
		{
			LOG << "Server mode";
			std::string buffer = "un message !";
			Network::Pipe pipe_server;
			pipe_server.listen(Common::PIPE_AUDIT_PIPE_NAME);
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
			if(pipe_client.connect(Common::PIPE_AUDIT_PIPE_NAME))
			{
				std::string buffer;
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
	CATCH_COMMON_EXCEPTION
	CATCH_UNKNOWN_EXCEPTION
	return EXIT_SUCCESS;
}

INNOCENCE_MAIN
