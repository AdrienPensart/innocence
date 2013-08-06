#include <network/UdpSocket.hpp>
#include <iostream>

int main(int argc, char * argv[])
{
	try
	{
		Network::UdpSocket server;
		server.listen(80);
		Network::Timeout deadline(0, 100);
		std::string buffer;

		while(true)
		{
			server.recv(buffer, deadline);
			if(buffer.size())
			{
				std::cout << buffer;
			}
		}
	}
	catch(std::exception& e)
	{
		std::cout << e.what();
	}
	catch(...)
	{
		std::cout << "Unknown exception";
	}
	return EXIT_SUCCESS;
}
