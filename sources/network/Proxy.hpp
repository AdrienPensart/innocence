#ifndef PROXY_H
#define PROXY_H

#include "Stream.hpp"

namespace Network
{

// utilise Boost::Thread

// le proxy a un but, faire passer tous les messages
class Proxy
{
    public:
        Proxy(Stream& one, Stream& two);
        void Run();
        void Stop();
        virtual ~Proxy();
    private:
        Stream * sone;
        Stream * stwo;
};

} /* Network */

#endif // PROXY_H
