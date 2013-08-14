#include "Service.hpp"
#include "SocketException.hpp"

namespace Network
{
	Host HostInfo::getLocalHostname()
	{
		char hostname[SIZEOFHOSTNAME];
		if(gethostname(hostname,SIZEOFHOSTNAME) == SOCKET_ERROR)
		{
			throw SocketException("gethostname failed");
		}
		return hostname;
	}

	Host HostInfo::getHostByName(const Host& name)
	{
		hostent * host = gethostbyname(name.c_str());
		if(!host)
		{
			throw SocketException("gethostbyname failed");
		}

		in_addr ** a=(struct in_addr **)host->h_addr_list;
		Host ip = inet_ntoa(**a);
		return ip;
	}
		
	Host HostInfo::getLocalIp()
	{
		return getHostByName(getLocalHostname());
	}

    Host HostInfo::getHostByAddr(const Host& ip)
	{
		hostent * host = gethostbyaddr(ip.c_str(), ip.size(), AF_INET);
		if(!host)
		{
			throw SocketException("gethostbyaddr failed");
		}

		Host name = host->h_name;
		return name;
	}

	ServiceInfo::Service ServiceInfo::getServByName(const Host& name, PROTO proto)
	{
		Service s;
		Host protocole;
		switch(proto)
		{
			case TCP:
				protocole = "tcp";
				break;
			case UDP:
				protocole = "udp";
				break;
			default:
			    return s;
		}
		servent * serv = getservbyname (name.c_str(), protocole.c_str());
		if(!serv)
		{
			throw SocketException("getservbyname failed");
		}

		s.name = serv->s_name;
		s.port = serv->s_port;
		s.proto = proto;
		return s;
	}

	ServiceInfo::Service ServiceInfo::getServByPort(const Port& port, PROTO proto)
	{
		Service s;
		Host protocole;
		switch(proto)
		{
			case TCP:
			    protocole = "tcp";
				break;
			case UDP:
			    protocole = "udp";
			    break;
			default:
			    return s;
		}
		servent * serv = getservbyport (port, protocole.c_str());
		if(!serv)
		{
			throw SocketException("getservbyport failed");
		}

		s.name = serv->s_name;
		s.port = serv->s_port;
		s.proto = proto;
		return s;
	}

} // Network
