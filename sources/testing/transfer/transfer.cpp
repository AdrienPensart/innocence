#include <common/Log.hpp>
#include <network/TcpServer.hpp>
#include <blaspheme/transfer/GenericTransfer.hpp>
#include <blaspheme/hash/Hash.hpp>
using namespace Blaspheme;
using namespace Network;

#include <iostream>
#include <cstdlib>
using namespace std;

class TransferShow : public TransferObserver
{
	public:

		void update(const TransferDescriptor& td)
		{
			cout << td.percentage << '\n';
		}
};

int main(int argc, char * argv[])
{
	LOG.setHeader("TEST TRANSFER");
    LOG.addObserver(new Common::LogToNetwork("127.0.0.1", 80));
	LOG.addObserver(new Common::LogToConsole);
	try
	{
		if(argc != 3)
		{
			LOG << "Nombre d'argument incorrect.";
			return EXIT_SUCCESS;
		}

		if(strcmp(argv[1], "-u") == 0)
		{
			// c'est un upload (mode client)
			TcpClient client;
			if(client.connect("127.0.0.1", 80))
			{
				LOG << "Connecte au serveur.";
				
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
				LOG << "Non Connecte au serveur.";
			}
		}
		else if(strcmp(argv[1], "-d") == 0)
		{
			// c'est un download (mode serveur)
			TcpServer server(80);
			TcpClient client;
			if(server.accept(client))
			{
				LOG << "Client connecte.";
                
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
				LOG << "Pas de client.";
			}
		}
		else
		{
			LOG << "Usage : transfer -[u|d|s] file";
		}
	}
	catch(...)
	{
		LOG << "Erreur d'origine inconnue.";
	}
	return EXIT_SUCCESS;
}
