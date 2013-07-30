#include "Service.hpp"

namespace Network
{
	Host HostInfo::getLocalHostname()
	{
		char * hostname = new char [SIZEOFHOSTNAME];
		gethostname(hostname,SIZEOFHOSTNAME);
		Host s = hostname;
		delete [] hostname;
		return s;
	}

	Host HostInfo::getHostByName(const Host& name)
	{
		hostent * host = gethostbyname(name.c_str());
		in_addr ** a=(struct in_addr **)host->h_addr_list;
		Host ip = inet_ntoa(**a);
		return ip;
	}
		
    Host HostInfo::getHostByAddr(const Host& ip)
	{
		hostent * host = gethostbyaddr(ip.c_str(), ip.size(), AF_INET);
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
		s.name = serv->s_name;
		s.port = serv->s_port;
		s.proto = proto;
		return s;
	}

} // Network
