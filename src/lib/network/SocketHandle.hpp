#pragma once

#include "Types.hpp"
/*
#include "Exception.hpp"
#include <string.h>
*/
namespace Network
{	
    /**
     *  Represente le descripteur de fichier d'une socket
     */
    class SocketHandle
    {        
        public:
            SocketHandle();
            SocketHandle(const SocketHandle&);
            
			virtual ~SocketHandle();

            /**
             * Manipulation du descripteur de fichier afin de pouvoir utiliser
             * les fonctions classiques de l'interface BSD (accès en lecture)
             */
            const SocketHandleImpl& getDescriptor() const ;
            /**
             * Manipulation du descripteur de fichier afin de pouvoir utiliser
             * les fonctions classiques de l'interface BSD (accès en écriture)
             */
            void setDescriptor(SocketHandleImpl& s);
            
            /**
             * Teste si le socket est dans un état valide, c'est à dire si le 
             * socket n'est pas ouvert
             */
			bool isValid() const ;
			
        protected:
        
            /**
             * Acquiert un socket en fonction du type du socket (TCP, UDP, etc...)
             */
            virtual void acquire()=0;
            
            // lance une exception si la socket n'est pas valide
            void test(void);
        
            /**
             *  Codes d'erreur associés à la valeur du descripteur système
             */
            enum
            {
                INVALID_SOCK = INVALID_SOCKET,
                SOCK_ERROR = SOCKET_ERROR,
                NOT_ACQUIRED = 0
            };
            
            /**
             * Handle specifique au système
             */
            SocketHandleImpl sockethandle;
            
        #ifdef WIN32
            /**
             * Initialise Winsock sur les plateformes Windows
             */
            class WSAInitializer
            {
                private:
                    WSADATA m_wsadata;
                public:
                    WSAInitializer();
                    ~WSAInitializer();
            };
            private:
                static WSAInitializer wsai;
        #endif
    };
} // Network
