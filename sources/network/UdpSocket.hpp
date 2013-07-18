#ifndef UDPSOCK_HPP
#define UDPSOCK_HPP

#include "Socket.hpp"
#include "Stream.hpp"
#include "Exception.hpp"

#include <string>

namespace Network
{
    /**
     * Cette classe permet de représenter un socket permettant
     * d'utiliser le protocole UDP sur le réseau
     */
    class UdpSocket : public Stream, public Socket
    {
        public:
            UdpSocket(const Port& argPort);
            UdpSocket();
        
            /**
             * Lie la socket au port spécifié en argument
             */
            void listen(const Port& argPort);
            
            virtual int send(const char * object, int sizeOfObject);
            virtual int send(const char * object, int sizeOfObject, Timeout to);
            virtual int send(const std::string& object){return Stream::send(object);}
            virtual int send(const std::string& object, Timeout to){return Stream::send(object, to);}
            
            virtual int recv(std::string& object, char delimiter, bool include=false){return Stream::recv(object, delimiter, include);}
            std::string recv(){return Stream::recv();}
            virtual int recv(char * object, int sizeOfObject);
            virtual int recv(char * object, int sizeOfObject, Timeout to);
            virtual int recv(std::string& object){return Stream::recv(object);}
            virtual int recv(std::string& object, Timeout to){return Stream::recv(object, to);}
            
            /**
             * Donne la taille du tampon de données, cela peut servir à ajuster
             * la taille des données que l'on veut envoyer en une seule fois.
             */
            virtual int getBufferDataSize();
        protected:
        
            // demande un nouvel handle socket au système
            void acquire();
    };
}

#endif
