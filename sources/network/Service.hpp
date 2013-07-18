#ifndef _SERVICE_
#define _SERVICE_

#include "Types.hpp"

namespace Network
{
    /**
     * Informations sur un hôte comme le nom de la machine locale, etc.
     */
    class HostInfo
    {
        public:
            /**
             * Renvoi du nom d'hôte de la machine locale
             */
		    static Host HostInfo::getLocalHostname()
		    {
			    char * hostname = new char [SIZEOFHOSTNAME];
			    gethostname(hostname,SIZEOFHOSTNAME);
			    Host s = hostname;
			    delete [] hostname;
			    return s;
		    }
		
            /**
             * Permet de connaître l'ip d'une machine à partir de son nom d'hote  
             */
            static Host HostInfo::getHostByName(const Host& name)
		    {
			    hostent * host = gethostbyname(name.c_str());
			    in_addr ** a=(struct in_addr **)host->h_addr_list;
			    Host ip = inet_ntoa(**a);
			    return ip;
		    }
		
            /**
             * Permet de connaître le nom d'hôte d'une machine à partir de son ip
             */
            static Host HostInfo::getHostByAddr(const Host& ip)
		    {
			    hostent * host = gethostbyaddr(ip.c_str(), ip.size(), AF_INET);
			    Host name = host->h_name;
			    return name;
		    }
        private:
            // taille par defaut du hostname
            enum {SIZEOFHOSTNAME = 64};
    };

    /**
     * Donne des informations sur les services que proposent une machine
     */
    class ServiceInfo
    {
	    public:
            /**
             * Représente un service sur une machine
             */
            typedef struct
            {
                Host name;
                Port port;
                int proto;
            } Service;

            /**
             * Le protocole que le service utilise
             */
		    enum PROTO {TCP, UDP};
		
		    static ServiceInfo::Service ServiceInfo::getServByName(const Host& name, PROTO proto)
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
		
		    /**
		     * Renvoi le nom du service à partir de son port associé
		     */
		    static ServiceInfo::Service ServiceInfo::getServByPort(const Port& port, PROTO proto)
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
    };
}

#endif
