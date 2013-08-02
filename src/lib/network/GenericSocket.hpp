#pragma once

#include "Types.hpp"
#include <string>
using std::string;

namespace Network
{
    class Client
    {
        protected:
            
    };
    
    class Server
    {
        protected:
            
    };
    
    template<class Mode>
    class Tcp
    {
        public:
            Tcp(){}
            int send();
            int recv();
    };
    
    template<class Mode>
    class Udp
    {
        public:
            Udp(){}
            int send();
            int recv();
    };
    
    class NonBlocking
    {
        public:
            bool writing_wait();
            bool reading_wait();
    };
    
    class Blocking
    {
        public:
            bool writing_wait();
            bool reading_wait();
    };
    
    class NoCiphering
    {
        public:
            void encode(string& str){}
            void decode(string& str){}
    };
    
    class XorCiphering
    {
        public:
            void encode(string& str);
            void decode(string& str);
    };
    
    class SslCiphering
    {
        public:
            void encode(string& str){}
            void decode(string& str){}
    };
    
    template <
                template <class> class  NetworkProtocol,
                class Mode,
                class BlockingPolicy=Blocking,
                class CipheringPolicy=NoCiphering
             >
    class GenericSocket : public NetworkProtocol<Mode>, public BlockingPolicy, public CipheringPolicy
    {
        public:
        
            GenericSocket()
            {
            }
            
            ~GenericSocket()
            {
            }
            
        private:
            
            
    };
} // Network
