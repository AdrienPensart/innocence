#pragma once

#include "Header.hpp"
#include <vector>

namespace Network
{
	class Packet
	{
		public:
			virtual void feed(std::vector<char>& buffer);
			virtual void forge(std::vector<char>& buffer);
			virtual size_t size();
			virtual size_t getMaxDataLength()=0;
			const std::string& getData();
			virtual void setData(const std::string& dataArg);
			std::vector<Header *> getHeaders();
			void addHeader(Header * header);
		private:
			std::vector<Header *> headers;
			std::string data;
	};

	struct UDPPacket : public Packet
	{
		UDPPacket();
		//virtual void forge(std::vector<char>& buffer);
		virtual size_t getMaxDataLength();
		virtual void setData(const std::string& dataArg);

		enum { MAX_UDP_PACKET_LENGTH = 576 };
		IPHeader * ip;
		UDPHeader * udp;
	};

	struct TCPPacket : public Packet
	{
		TCPPacket();

		virtual size_t getMaxDataLength();
		virtual void setData(const std::string& dataArg);

		enum { MAX_TCP_PACKET_LENGTH = 65535 };
		IPHeader * ip;
		TCPHeader * tcp;
	};

	struct ICMPPacket : public Packet
	{
		ICMPPacket();
		virtual size_t getMaxDataLength();
		virtual void setData(const std::string& dataArg);

		enum { MAX_ICMP_PACKET_LENGTH = 1000 };
		IPHeader * ip;
		ICMPHeader * icmp;
	};
} // Network
