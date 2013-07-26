#ifndef _PROXY_
#define _PROXY_

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
} // Network

#endif // _PROXY_

