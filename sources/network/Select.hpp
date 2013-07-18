#ifndef SELECT_HPP
#define SELECT_HPP

#include "Socket.hpp"
#include "Exception.hpp"
#include "Timeout.hpp"

namespace Network
{  
    /**
     * Gestion d'un ensemble de sockets pour la détection d'événement asynchrone
     */
    class SelectSocket
    {
        private:
            enum SelectType { READFS, WRITEFS, EXCEPTFS };
        public:
            /**
             * Atteint le temps to que le socket socket soit prêt à lire un flux
             * d'octets depuis la connexion
             */
            static bool SelectOnRead(Socket& socket, Timeout& to);
            /**
             * Atteint le temps to que le socket socket soit prêt à écrire un flux
             * d'octets vers la connexion
             */
		    static bool SelectOnWrite(Socket& socket, Timeout& to);
		    /**
             * Atteint le temps to que le socket socket ai reçu des données
             * hors bande
             */
		    static bool SelectOnExcept(Socket& socket, Timeout& to);
	    private:
            /**
             * Fonction bas niveau pour la gestion des événements asynchrones
             */
            
		    static bool LowLevelSelect(Socket& socket,SelectType type,Timeout & to);
    };
}

#endif
