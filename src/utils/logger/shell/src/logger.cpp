#include <network/UdpSocket.hpp>
#include <iostream>

int main(int argc, char * argv[])
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
	return EXIT_SUCCESS;
}
