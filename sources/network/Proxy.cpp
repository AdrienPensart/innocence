#include "Proxy.hpp"

namespace Network
{

Proxy::Proxy(Stream& one, Stream& two)
:sone(&one),
 stwo(&two)
{
    //ctor
}

Proxy::~Proxy()
{
    //dtor
}

void Proxy::Run()
{
    // créer deux threads, chaque thread, quand il reçoit des donnes de l'un ou
    // de l'autre des Stream, retransmet ce qu'il a reçu a l'autre stream
    
}

void Proxy::Stop()
{
}

}
