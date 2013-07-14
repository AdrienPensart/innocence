#ifndef _SOCKTYPE_
#define _SOCKTYPE_

#include <string>

// en-tete des bibliotheques reseau de la plateforme ciblee :
#ifdef WIN32
    #ifdef WIN32_LEAN_AND_MEAN
        #include <winsock2.h>
    #else
        #include <windows.h>
    #endif

	#ifdef _MSC_VER
        #pragma comment(lib,"ws2_32.lib")
	#endif
#else // on fait une supposition que la machine est sur UNIX
	#include <errno.h>
    #include <netdb.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <sys/ioctl.h>
    #include <fcntl.h>
    #define ioctlsocket ioctl
    #define closesocket(s) close(s)
    typedef int OptionIoctl;
    #define SOCKET_ERROR -1
    #define INVALID_SOCKET -1
    typedef int TypeSocket;
#endif

namespace Network
{
#ifdef WIN32
    // on simplifies quelques incompatibilites
    typedef int socklen_t;
    typedef SOCKADDR_IN sockaddr_in;
    typedef SOCKADDR sockaddr;
    typedef SOCKET TypeSocket;
    typedef unsigned long OptionIoctl;
#endif

    /**
     * Handle multiplateforme représentant un socket pour le système
     */
    typedef TypeSocket SocketHandleImpl;

    /**
     * Chaine de caractere représentant soit l'ip d'une machine, soit son nom
     */
    typedef std::string Host;

    /**
     * Entier représentant le numéro de port d'un service sur les protocoles
     * UDP et TCP
     */
    typedef unsigned short int Port;

    /**
     * Structure d'adresse pour toutes les familles de protocoles
     */
    typedef sockaddr Addr;

    typedef sockaddr_in AddrIn;
} /* Network */

#endif // _SOCKTYPE_
