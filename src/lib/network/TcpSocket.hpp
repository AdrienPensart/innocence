#pragma once

#include "Socket.hpp"

namespace Network
{

/**
 * Le protocole TCP est accessible via la classe TcpSocket, qui donne acces a
 * toutes les fonctions de l'interface standard, mais avec la facilité en plus
 * de l'encapsulation objet !
 */
class TcpSocket : public Socket
{
    protected:
        virtual void acquire();
};

} // Network
