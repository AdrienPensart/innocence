#pragma once

// en-tete des bibliotheques reseau de la plateforme ciblee :
#ifdef WIN32
	#define NOMINMAX
	#include <winsock2.h>
	#include <windows.h>
	#pragma comment(lib, "ws2_32.lib")
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
	typedef int DWORD;
#endif