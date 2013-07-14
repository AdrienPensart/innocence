#ifndef _NETWORK_CPP_LIB
#define _NETWORK_CPP_LIB

#include "Pipe.hpp"
#include "TcpSocket.hpp"
#include "UdpSocket.hpp"

// Compatibilit� avec le compilateur de Microsoft
#ifdef _MSC_VER
    #pragma comment(lib,"WS2_32.LIB")
#endif

#endif
