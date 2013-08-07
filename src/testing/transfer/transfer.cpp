#include <common/Log.hpp>
#include <network/TcpServer.hpp>
#include <blaspheme/transfer/GenericTransfer.hpp>
#include <blaspheme/hash/Hash.hpp>
using namespace Blaspheme;
using namespace Network;

#include <Innocence.hpp>

class TransferShow : public TransferObserver
{
	public:

		void update(const TransferDescriptor& td)
		{
			LOG << toString(td.percentage);
		}
};

int main(int argc, char * argv[])
{
	LOG.setIdentity(Innocence::identity);
    LOG.addObserver(new Common::LogToCollector);
	LOG.addObserver(new Common::LogToConsole);
	try
	{
		if(argc != 3)
		{
			LOG << "Bad argument number";
			return EXIT_SUCCESS;
		}

		if(strcmp(argv[1], "-u") == 0)
		{
			// c'est un upload (mode client)
			TcpClient client;
			if(client.connect("127.0.0.1", 80))
			{
				LOG << "Connected to server";
				
				//Upload upload(argv[2], client);
				//upload.addObserver(new TransferShow);
				//upload.launch();
                
                File input(argv[2]);
                NetworkStream output(client);
                FileUpload upload(input, output);
                upload.launch();
			}
			else
			{
				LOG << "Connected to server";
			}
		}
		else if(strcmp(argv[1], "-d") == 0)
		{
			// c'est un download (mode serveur)
			TcpServer server(80);
			TcpClient client;
			if(server.accept(client))
			{
				LOG << "Client connected";
                
				//Download download(argv[2], client);
				//download.addObserver(new TransferShow);
				//download.launch();
                
                NetworkStream input(client);
                File output(argv[2]);
                FileDownload download(input, output);
                download.launch();
			}
			else
			{
				LOG << "No client";
			}
		}
		else
		{
			LOG << "Usage : transfer -[u|d|s] file";
		}
	}
	CATCH_UNKNOWN_EXCEPTION
	return EXIT_SUCCESS;
}
