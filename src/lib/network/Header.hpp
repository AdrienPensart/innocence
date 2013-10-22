#pragma once

#include <string>

namespace Network
{
	struct Header
	{
		virtual void computeChecksum()=0;
		virtual size_t getSize()=0;
		virtual void * getImpl()=0;
		virtual void feed(const char * data, size_t length)=0;
	};

	// TODO : little indian and big indian handling
	struct IPHeader : public Header
	{
		struct Impl
		{
			unsigned char hlength:4;    // Header length (in 32-bit words), minimum 5
			unsigned char version:4;    // IPv4 version
			unsigned char tos;          // IP type of service
			unsigned short tlength;     // Total length, header + data, (in bytes)
			unsigned short id;          // Unique identifier for fragmentation, usually process id

			unsigned short fragmentation;
		
			unsigned char ttl;          // Time to live
			unsigned char protocol;     // Protocol(TCP,UDP etc)
			unsigned short checksum;    // IP checksum
		
			unsigned int saddr;       // Source address
			unsigned int daddr;      // Source address
		};

		IPHeader();
		Impl header;
		virtual void feed(const char * data, size_t length);
		virtual void computeChecksum();
		virtual size_t getSize();
		virtual void * getImpl();
	};
	
	struct ICMPHeader : public Header
	{
		struct Impl
		{
			unsigned char   type;
			unsigned char   code;
			unsigned short  checksum;
			unsigned short  id;
			unsigned short  seq;
		};
		Impl header;
		const std::string& data;

		ICMPHeader(const std::string& dataArg);
		virtual void feed(const char * data, size_t length);
		virtual void computeChecksum();
		virtual size_t getSize();
		virtual void * getImpl();
		unsigned int timestamp;
	};

	struct UDPHeader : public Header
	{
		struct Impl
		{
			unsigned short sport;
			unsigned short dport;
			unsigned short length;
			unsigned short checksum;
		};

		UDPHeader(const IPHeader& ipArg, const std::string& data);
		virtual void feed(const char * data, size_t length);
		virtual void computeChecksum();
		virtual size_t getSize();
		virtual void * getImpl();

		Impl header;
		const IPHeader& ip;
		const std::string& data;
	};

	struct TCPHeader : public Header
	{
		struct Impl
		{
			unsigned short sport;   // source port
			unsigned short dport;     // destination port
			unsigned int seq;        // sequence number - 32 bits
			unsigned int acknowledge;     // acknowledgement number - 32 bits
 
			unsigned char ns :1;          //Nonce Sum Flag Added in RFC 3540.
			unsigned char reserved_part1:3; //according to rfc
			unsigned char offset:4;    // The number of 32-bit words in the TCP header. This indicates where the data begins. The length of the TCP header	is always a multiple of 32 bits.
 
			unsigned char fin :1; //Finish Flag
			unsigned char syn :1; //Synchronise Flag
			unsigned char rst :1; //Reset Flag
			unsigned char psh :1; //Push Flag
			unsigned char ack :1; //Acknowledgement Flag
			unsigned char urg :1; //Urgent Flag
 
			unsigned char ecn :1; //ECN-Echo Flag
			unsigned char cwr :1; //Congestion Window Reduced Flag
 
			unsigned short window; // window
			unsigned short checksum; // checksum
			unsigned short urgent_pointer; // urgent pointer
		};

		TCPHeader(const IPHeader& ipArg, const std::string& data);
		virtual void feed(const char * data, size_t length);
		virtual void computeChecksum();
		virtual size_t getSize();
		virtual void * getImpl();

		Impl header;
		const IPHeader& ip;
		const std::string& data;
	};

	/*
	typedef struct ipv6_header
	{
		unsigned long   ipv6_vertcflow;        // 4-bit IPv6 version, 8-bit traffic class, 20-bit flow label
		unsigned short  ipv6_payloadlen;    // payload length
		unsigned char   ipv6_nexthdr;          // next header protocol value
		unsigned char   ipv6_hoplimit;         // TTL
		struct in6_addr ipv6_srcaddr;          // Source address
		struct in6_addr ipv6_destaddr;        // Destination address
	} IPV6_HDR, *PIPV6_HDR, FAR * LPIPV6_HDR, IPV6Header, IPV6_HEADER;

	typedef struct ipv6_fragment_header
	{
		unsigned char   ipv6_frag_nexthdr;
		unsigned char   ipv6_frag_reserved;
		unsigned short  ipv6_frag_offset;
		unsigned long   ipv6_frag_id;
	} IPV6_FRAGMENT_HDR, *PIPV6_FRAGMENT_HDR, FAR * LPIPV6_FRAGMENT_HDR, IPV6FragmentHeader, IPV6_FRAGMENT_HEADER;
	*/
} // Network
