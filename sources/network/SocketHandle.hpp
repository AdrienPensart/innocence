#ifndef SOCKETHANDLE_H
#define SOCKETHANDLE_H

#include "Types.hpp"
#include "Exception.hpp"
#include <string.h>

namespace Network
{
    #ifdef WIN32
        /**
         *  Exception lancée si l'initialisation de Winsock sur les plateformes
         *  Windows échoue
         */
        class WinsockInitExcept : public Exception{};
    #endif
    
	class SocketHandle;

	class SocketException : public Exception
	{
		public:
	        
			SocketException(SocketHandleImpl sock, const std::string& argMsgError)
			{
				#ifdef WIN32
					msgError = argMsgError + " " + intToString(WSAGetLastError()) + '\n';
				#else
					msgError = argMsgError + " " + strerror(errno) + '\n';
				#endif
			}
			
			virtual ~SocketException()throw(){}
			
			const SocketHandleImpl& getValue()
			{
				return socketValue;
			}
			
			const char * what()
			{
				return msgError.c_str();
			}
			
			static std::string getLastError()
			{
				#ifdef WIN32
					return intToString(WSAGetLastError());
				#else
					return intToString(errno);
				#endif
			}
			
			static int getCodeError()
			{
				#ifdef WIN32
					return WSAGetLastError();
				#else
					return errno;
				#endif
			}

		protected:
	        
			SocketHandleImpl socketValue;
			std::string msgError;
	};

	class UnexpectedEvent : public SocketException
	{
		public:
			UnexpectedEvent(SocketHandleImpl argSock, const std::string& msgError):SocketException(argSock, msgError){}
	};

	class InvalidBlocking : public UnexpectedEvent
	{
		public:
			InvalidBlocking(SocketHandleImpl argSock, const std::string& msgError):UnexpectedEvent(argSock, msgError){}
	};

	class InvalidSock : public UnexpectedEvent
	{
		public:
			InvalidSock(SocketHandleImpl argSock, const std::string& msgError):UnexpectedEvent(argSock, msgError){}
	};

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
}

#endif
