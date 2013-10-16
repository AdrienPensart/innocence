#pragma once

#include <string>
#include <common/WindowsWrapper.hpp>

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
     * Handle multiplateforme repr�sentant un socket pour le syst�me
     */
    typedef TypeSocket SocketHandleImpl;

    /**
     * Chaine de caractere repr�sentant soit l'ip d'une machine, soit son nom
     */
    typedef std::string Host;

    /**
     * Entier repr�sentant le num�ro de port d'un service sur les protocoles
     * UDP et TCP
     */
    typedef unsigned short int Port;

    /**
     * Structure d'adresse pour toutes les familles de protocoles
     */
    typedef sockaddr Addr;

    typedef sockaddr_in AddrIn;
} // Network
