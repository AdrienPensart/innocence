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
    // cr�er deux threads, chaque thread, quand il re�oit des donnes de l'un ou
    // de l'autre des Stream, retransmet ce qu'il a re�u a l'autre stream
    
}

void Proxy::Stop()
{
}

}
