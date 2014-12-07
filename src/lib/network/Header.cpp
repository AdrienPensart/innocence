#include "Header.hpp"
#include <log/Log.hpp>
#include <cstring>

namespace Network
{
	IPHeader::IPHeader()
	{
		header.hlength = 5;
		header.version = 4;
		header.tos = 0;
		header.id = 0;
		header.fragmentation = 0;
		header.ttl = 8;
	}

	void IPHeader::feed(const char * data, size_t length)
	{
		#ifdef WIN32
			memcpy_s(&header, getSize(), data, length);
		#else
			memcpy(&header, data, length);
		#endif
	}

	void IPHeader::computeChecksum()
	{
		header.checksum = 0;
		unsigned long sum = 0;
		size_t len = getSize();
		unsigned short * ip = (unsigned short*)&header;
		while(len > 1)
		{
			sum += *(ip)++;
			if(sum & 0x80000000)
				sum = (sum & 0xFFFF) + (sum >> 16);
			len -= 2;
		}
		if(len)
		{
			sum += (unsigned short)*(unsigned char *)ip;
		}

		while(sum >> 16)
		{
			sum = (sum & 0xFFFF) + (sum >> 16);
		}
		header.checksum = (unsigned short)~sum;
	}

	size_t IPHeader::getSize()
	{
		return sizeof(Impl);
	}

	void * IPHeader::getImpl()
	{
		return &header;
	}

	UDPHeader::UDPHeader(const IPHeader& ipArg, const std::string& dataArg) :
		ip(ipArg),
		data(dataArg)
	{
	}

	void UDPHeader::feed(const char * data, size_t length)
	{
		#ifdef WIN32
			memcpy_s(&header, getSize(), data, length);
		#else
			memcpy(&header, data, length);
		#endif
	}

	void * UDPHeader::getImpl()
	{
		return &header;
	}

	void feed(const char * data, size_t length)
	{
	}

	size_t UDPHeader::getSize()
	{
		return sizeof(Impl);
	}

	void UDPHeader::computeChecksum()
	{
		unsigned int sum = 0;
		sum += header.sport;
		if (sum & 0x80000000)
			sum = (sum & 0xFFFF) + (sum >> 16);

		sum += header.dport;
		if (sum & 0x80000000)
			sum = (sum & 0xFFFF) + (sum >> 16);

		header.length = htons(getSize() + data.size());
		sum += header.length;
		if (sum & 0x80000000)
			sum = (sum & 0xFFFF) + (sum >> 16);

		// data
		const unsigned short * buf = (unsigned short*)data.c_str();
		size_t len = data.size();
		while(len > 1)
		{
			sum += *buf++;
			if (sum & 0x80000000)
				sum = (sum & 0xFFFF) + (sum >> 16);
			len -= 2;
		}
		// data padding
		if ( len & 1 )
		{
			sum += *(buf);
		}

		// pseudo header source ip
		unsigned short * ip_src = (unsigned short *)&ip.header.saddr;
		sum += *(ip_src++);
		sum += *ip_src;
		// pseudo header destination ip
		unsigned short * ip_dst = (unsigned short *)&ip.header.daddr;
		sum += *(ip_dst++);
		sum += *ip_dst;
		// protocole
		sum += htons(IPPROTO_UDP);
		// UDP size
		sum += htons(8 + data.size());

		while (sum >> 16)
		{
			sum = (sum & 0xFFFF) + (sum >> 16);
		}
		header.checksum = (unsigned short)~sum;
	}

	ICMPHeader::ICMPHeader(const std::string& dataArg) :
		data(dataArg)
	{
	}

	void * ICMPHeader::getImpl()
	{
		return &header;
	}

	void ICMPHeader::feed(const char * data, size_t length)
	{
		#if WIN32
			memcpy_s(&header, getSize(), data, length);
		#else
			memcpy(&header, data, length);
		#endif
	}

	void ICMPHeader::computeChecksum()
	{
		unsigned long cksum=0;

		cksum += header.type;
		cksum += header.code;
		cksum += header.id;
		cksum += header.seq;

		unsigned short * buffer = (unsigned short*)data.c_str();
		size_t size = data.size();
		while(size > 1)
		{
			cksum += *buffer++;
			size -= sizeof(unsigned short);
		}
		if(size)
		{
			cksum += *buffer;
		}

		cksum = (cksum >> 16) + (cksum & 0xffff);
		cksum += (cksum >>16);
		header.checksum = (unsigned short)~cksum;
	}

	size_t ICMPHeader::getSize()
	{
		return sizeof(Impl);
	}

	TCPHeader::TCPHeader(const IPHeader& ipArg, const std::string& dataArg) :
		ip(ipArg),
		data(dataArg)
	{
	}

	void TCPHeader::feed(const char * data, size_t length)
	{
		#ifdef WIN32
			memcpy_s(&header, getSize(), data, length);
		#else
			memcpy(&header, data, length);
		#endif
	}

	void TCPHeader::computeChecksum()
	{

	}

	size_t TCPHeader::getSize()
	{
		return sizeof(Impl);
	}

	void * TCPHeader::getImpl()
	{
		return &header;
	}

} // Network
