#include "Proxy.hpp"

namespace Network
{
    Proxy::Proxy(Stream& one, Stream& two)
    :sone(&one),
     stwo(&two)
    {
    }

    Proxy::~Proxy()
    {
    }

    void Proxy::Run()
    {
    }

    void Proxy::Stop()
    {
    }
}
