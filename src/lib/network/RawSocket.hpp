#pragma once

#include "Socket.hpp"

namespace Network
{
	// TODO : little indian and big indian handling
	struct IPHeader
	{
		unsigned char hlength:4;    // Header length (in 32-bit words), minimum 5
		unsigned char version:4;   // IPv4 version
		unsigned char tos;          // IP type of service
		unsigned short tlength;     // Total length, header + data, (in bytes)
		unsigned short id;          // Unique identifier for fragmentation, usually process id

		// Fragmentation flags
		unsigned short frag_offset :13; //fragment offset (in 64-bit words)
		unsigned char frag_more :1;
		unsigned char frag_dont :1;
		unsigned char frag_unused :1;

		unsigned char ip_frag_offset1; 
 
		unsigned char ip_ttl;          // Time to live
		unsigned char ip_protocol;     // Protocol(TCP,UDP etc)
		unsigned short ip_checksum;    // IP checksum
		unsigned int ip_srcaddr;       // Source address
		unsigned int ip_destaddr;      // Source address
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
	
	typedef struct udp_header
	{
		USHORT source_port;
		USHORT dest_port;
		USHORT length;
		USHORT checksum;
	} UDP_HDR, *PUDP_HDR, FAR * LPUDP_HDR, UDPHeader, UDP_HEADER;

	struct TCPHeader
	{
		unsigned short source_port;   // source port
		unsigned short dest_port;     // destination port
		unsigned int sequence;        // sequence number - 32 bits
		unsigned int acknowledge;     // acknowledgement number - 32 bits
 
		unsigned char ns :1;          //Nonce Sum Flag Added in RFC 3540.
		unsigned char reserved_part1:3; //according to rfc
		unsigned char data_offset:4;    // The number of 32-bit words in the TCP header. This indicates where the data begins. The length of the TCP header	is always a multiple of 32 bits.
 
		unsigned char fin :1; //Finish Flag
		unsigned char syn :1; //Synchronise Flag
		unsigned char rst :1; //Reset Flag
		unsigned char psh :1; //Push Flag
		unsigned char ack :1; //Acknowledgement Flag
		unsigned char urg :1; //Urgent Flag
 
		unsigned char ecn :1; //ECN-Echo Flag
		unsigned char cwr :1; //Congestion Window Reduced Flag
 
		////////////////////////////////
 
		unsigned short window; // window
		unsigned short checksum; // checksum
		unsigned short urgent_pointer; // urgent pointer
	};

	typedef struct icmp_header {
		unsigned char type;          // ICMP packet type
		unsigned char code;          // Type sub code
		unsigned short checksum;
		unsigned short id;
		unsigned short seq;
		unsigned int timestamp;    // not part of ICMP, but we need it
	} ICMP_HDR, *PICMP_HDR, FAR * LPICMP_HDR, ICMPHeader, ICMP_HEADER;
	*/
	class RawSocket : public Socket
	{
		public:
			enum Proto {UDP, TCP, ICMP};
			RawSocket(Proto proto)
			{

			}

		protected:
			Proto proto;
			virtual void acquire();
	};
} // Network
