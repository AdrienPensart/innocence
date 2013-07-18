#ifndef SELECT_HPP
#define SELECT_HPP

#include "Socket.hpp"
#include "Exception.hpp"
#include "Timeout.hpp"

namespace Network
{  
    /**
     * Gestion d'un ensemble de sockets pour la d�tection d'�v�nement asynchrone
     */
    class SelectSocket
    {
        private:
            enum SelectType { READFS, WRITEFS, EXCEPTFS };
        public:
            /**
             * Atteint le temps to que le socket socket soit pr�t � lire un flux
             * d'octets depuis la connexion
             */
            static bool SelectOnRead(Socket& socket, Timeout& to);
            /**
             * Atteint le temps to que le socket socket soit pr�t � �crire un flux
             * d'octets vers la connexion
             */
		    static bool SelectOnWrite(Socket& socket, Timeout& to);
		    /**
             * Atteint le temps to que le socket socket ai re�u des donn�es
             * hors bande
             */
		    static bool SelectOnExcept(Socket& socket, Timeout& to);
	    private:
            /**
             * Fonction bas niveau pour la gestion des �v�nements asynchrones
             */
            
		    static bool LowLevelSelect(Socket& socket,SelectType type,Timeout & to);
    };
}

#endif
