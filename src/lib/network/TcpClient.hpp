#pragma once

#include "TcpSocket.hpp"
#include "Stream.hpp"

namespace Network
{
    /**
     * Représente une connexion renvoyée par un serveur Tcp par Accept(), ou bien
     * cette classe peut servir à se connecter vers un serveur grâce à la fonction
     * Connect()
     */
    class TcpClient : public Stream, public TcpSocket
    {
        public:
		    bool connect(const Host& argIp, const Port& argPort);

		    #ifdef _WINDOWS_
			    bool connect(const Host& argIp, const Port argPort, Timeout to);
		    #endif

            void reset();
		    void disconnect();

		    virtual int send(const char * object, int sizeOfObject);
		    virtual int send(const char * object, int sizeOfObject, Timeout to);
		    virtual int send(const std::string& object);
            virtual int send(const std::string& object, Timeout to);

		    virtual int recv(char * object, int sizeOfObject);
		    virtual int recv(char * object, int sizeOfObject, Timeout to);
		    virtual int recv(std::string& object);
            virtual int recv(std::string& object, Timeout to);
            std::string recv();
            /**
             * Reception d'une chaine de caractere jusqu'au caractère c
             */
            virtual int recv(std::string& object, char delimiter, bool include=false);
			virtual int recv(std::string& object, char delimiter, Timeout to, bool include=false);

		    bool setKeepAlive(int aliveToggle);
		    virtual int getBufferDataSize();
    };
} // Network
