#ifndef PROXY_H
#define PROXY_H

#include "Stream.hpp"

namespace Network
{
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
}

#endif
