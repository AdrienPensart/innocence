#ifndef TCPCLIENT_HPP
#define TCPCLIENT_HPP

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
			bool connect(const std::string& argIp, const Port argPort, Timeout to);
		#endif

        void reset();
		void disconnect();

		virtual int send(const char * object, int sizeOfObject);
		virtual int send(const char * object, int sizeOfObject, Timeout to);
		virtual int send(const std::string& object){return Stream::send(object);}
        virtual int send(const std::string& object, Timeout to){return Stream::send(object, to);}

		virtual int recv(char * object, int sizeOfObject);
		virtual int recv(char * object, int sizeOfObject, Timeout to);
		virtual int recv(std::string& object){return Stream::recv(object);}
        virtual int recv(std::string& object, Timeout to){return Stream::recv(object, to);}
        std::string recv(){return Stream::recv();}
        /**
         * Reception d'une chaine de caractere jusqu'au caractère c
         */
        virtual int recv(std::string& object, char delimiter, bool include=false){return Stream::recv(object, delimiter, include);}

		bool setKeepAlive(int aliveToggle);
		virtual int getBufferDataSize();
};

} /* Network */

#endif // TCPCLIENT_HPP

