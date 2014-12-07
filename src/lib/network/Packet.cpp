#include "Packet.hpp"
#include <log/Log.hpp>

#include <cstring>

namespace Network
{
	size_t Packet::size()
	{
		size_t s = data.size();
		for(size_t index = 0; index < headers.size(); index++)
			s += headers[index]->getSize();
		return s;
	}

	std::vector<Header *> Packet::getHeaders()
	{
		return headers;
	}

	void Packet::addHeader(Header * header)
	{
		headers.push_back(header);
	}

	const std::string& Packet::getData()
	{
		return data;
	}

	void Packet::setData(const std::string& dataArg)
	{
		data = dataArg;
	}

	void Packet::feed(std::vector<char>& buffer)
	{
		size_t offset = 0;
		for(size_t index = 0; index < headers.size(); index++)
		{
			Header * current = headers[index];
			LOG << "Offset : " + Common::toString(offset) + ", size to feed : " + Common::toString(current->getSize());
			current->feed(&buffer[0]+offset, current->getSize());
			offset += current->getSize();
		}
		data = std::string(&buffer[0]+offset, buffer.size()-offset);
	}

	void Packet::forge(std::vector<char>& buffer)
	{
		buffer.resize(size());
		size_t offset = 0;
		for(size_t index = 0; index < headers.size(); index++)
		{
			Header * current = headers[index];
			#if WIN32
				memcpy_s(&buffer[0]+offset, current->getSize(), current->getImpl(), current->getSize());
			#else
				memcpy(&buffer[0]+offset, current->getImpl(), current->getSize());
			#endif
			offset += current->getSize();
		}
		#if WIN32
			memcpy_s(&buffer[0] + offset, getMaxDataLength(), getData().c_str(), getData().size());
		#else
			memcpy(&buffer[0] + offset, getData().c_str(), getData().size());
		#endif
	}

	UDPPacket::UDPPacket()
	{
		ip = new IPHeader;
		udp = new UDPHeader(*ip, getData());
		ip->header.protocol = IPPROTO_UDP;

		addHeader(ip);
		addHeader(udp);
	}

	void UDPPacket::setData(const std::string& dataArg)
	{
		Packet::setData(dataArg);

		ip->header.tlength = size();
		ip->computeChecksum();

		udp->computeChecksum();
		LOG << "IP Checksum : " + Common::toString(ip->header.checksum);
		LOG << "UDP Checksum : " + Common::toString(udp->header.checksum);
	}

	size_t UDPPacket::getMaxDataLength()
	{
		return MAX_UDP_PACKET_LENGTH - (ip->getSize() + udp->getSize());
	}

	ICMPPacket::ICMPPacket()
	{
		ip = new IPHeader;
		icmp = new ICMPHeader(getData());
		ip->header.protocol = IPPROTO_ICMP;

		addHeader(ip);
		addHeader(icmp);
	}

	size_t ICMPPacket::getMaxDataLength()
	{
		return MAX_ICMP_PACKET_LENGTH - (ip->getSize() + icmp->getSize());
	}

	void ICMPPacket::setData(const std::string& dataArg)
	{
		Packet::setData(dataArg);

		ip->header.tlength = size();
		ip->computeChecksum();
		icmp->computeChecksum();
	}

	TCPPacket::TCPPacket()
	{
		ip = new IPHeader;
		tcp = new TCPHeader(*ip, getData());

		// The destination port, we accept through command line
		tcp->header.seq = htonl(1);
		tcp->header.acknowledge = 0;
		tcp->header.offset = 5;
		tcp->header.syn = 1;
		tcp->header.ack = 0;
		tcp->header.window = htons(32767);
		tcp->header.checksum = 0;
		tcp->header.urgent_pointer = 0;

		ip->header.protocol = IPPROTO_TCP;

		addHeader(ip);
		addHeader(tcp);
	}

	size_t TCPPacket::getMaxDataLength()
	{
		return MAX_TCP_PACKET_LENGTH - (ip->getSize() + tcp->getSize());
	}

	void TCPPacket::setData(const std::string& dataArg)
	{
		Packet::setData(dataArg);

		ip->header.tlength = size();
		ip->computeChecksum();
		tcp->computeChecksum();
	}
} // Network
