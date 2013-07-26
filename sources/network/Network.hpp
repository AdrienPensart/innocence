#ifndef _NETWORK_
#define _NETWORK_

#include "Pipe.hpp"
#include "TcpSocket.hpp"
#include "UdpSocket.hpp"

// Compatibilité avec le compilateur de Microsoft
#ifdef _MSC_VER
    #pragma comment(lib,"WS2_32.LIB")
#endif

#endif // _NETWORK_
