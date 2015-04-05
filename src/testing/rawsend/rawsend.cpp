#include <log/Log.hpp>
#include <common/Innocence.hpp>
#include <common/ParseOptions.hpp>
#include <audit/Audit.hpp>
#include <network/RawSocket.hpp>
#include <system/Process.hpp>
#include <iostream>
using namespace std;

int submain(int argc, char ** argv) {
	try {
		LOG.setIdentity(Common::identity);
		Common::ParseOptions(argc, argv);
		/*
		Network::RawSocket rs(IPPROTO_UDP);
		rs.setDestInfo("192.168.0.1", 8001);

		Network::UDPPacket packet;
		packet.ip->header.saddr = inet_addr("192.168.0.219");
		packet.ip->header.daddr = rs.getAddr().sin_addr.s_addr;

		packet.udp->header.sport = htons(atoi("8000"));
		packet.udp->header.dport = rs.getAddr().sin_port;

		packet.setData("helloworld");
		*/
		Network::RawSocket rs(IPPROTO_TCP);
		rs.setDestInfo("192.168.0.1", 8001);

		Network::TCPPacket packet;
		packet.ip->header.saddr = inet_addr("192.168.0.219");
		packet.ip->header.daddr = rs.getAddr().sin_addr.s_addr;

		packet.tcp->header.sport = htons(atoi("8000"));
		packet.tcp->header.dport = rs.getAddr().sin_port;

		packet.setData("helloworld");

		int bytesSent = rs.send(packet);
		if(bytesSent < 0) {
			LOG << "Error : " + Network::SocketException::getLastError();
		} else {
			LOG << "Bytes sent : " + Common::toString(bytesSent);
		}
		/*
		Network::RawSocket rs(IPPROTO_ICMP);
		rs.setDestInfo("192.168.0.1", 8001);

		Network::ICMPPacket echo;
		echo.ip->header.saddr = inet_addr("192.168.0.219");
		echo.ip->header.daddr = rs.getAddr().sin_addr.s_addr;

		echo.icmp->header.type = 8;
		echo.icmp->header.code = 0;
		echo.icmp->header.id = 0;
		echo.icmp->header.seq = 1;

		echo.setData("helloworld");

		Network::RawSocket server(IPPROTO_ICMP);
		server.listen(0);

		int bytesSent = rs.send(echo);
		if(bytesSent < 0)
		{
			LOG << "Error : " + Network::SocketException::getLastError();
		}
		else
		{
			LOG << "Bytes sent : " + Common::toString(bytesSent);
		}

		int bytesRecv;
		Network::ICMPPacket reply;
		bytesRecv = server.recv(reply);
		LOG << "Bytes received : " + Common::toString(bytesRecv);

		if(reply.icmp->header.type == 0)
		{
			LOG << "Data of echo reply : " + reply.getData();
		}
		else
		{
			LOG << "Not a reply ICMP packet";
		}
		*/
		/*
		Network::UDPHeader * udp = (Network::UDPHeader *)buffer;
		char * data = buffer + sizeof(Network::UDPHeader);
		size_t maxDataLength = MAX_UDP_PACKET_LENGTH - sizeof(Network::UDPHeader);
		memcpy_s(data, maxDataLength, helloworld, strlen(helloworld));
		size_t bufferLength = sizeof(Network::UDPHeader) + strlen(helloworld);

		udp->sport = htons(atoi("8000"));
		udp->dport = htons(atoi("8001"));
		udp->length = htons(sizeof(Network::UDPHeader) + strlen(helloworld));
		udp->checksum = Network::UDPChecksum(;

		//while(true)
		{
			int bytesSent = rs.send(buffer, bufferLength);
			if(bytesSent < 0)
			{
				LOG << "Error : " + Network::SocketException::getLastError();
			}
			else
			{
				LOG << "Bytes sent : " + Common::toString(bytesSent);
			}
			//system("pause");
		}
		*/
	}
	CATCH_COMMON_EXCEPTION
	CATCH_UNKNOWN_EXCEPTION
	return EXIT_SUCCESS;
}

INNOCENCE_MAIN
