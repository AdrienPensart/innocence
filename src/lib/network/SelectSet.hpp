#pragma once

#include "Socket.hpp"
#include "Timeout.hpp"

#include <vector>

namespace Network
{
	typedef std::vector<Socket*> SocketSet;

    /**
     * Gestion d'un ensemble de sockets pour la détection d'événement asynchrone
     */
    class SelectSet
    {
        private:

            enum SelectType { READFS, WRITEFS, EXCEPTFS };

        public:

            /**
             * Atteint le temps to que le socket socket soit prêt à lire un flux
             * d'octets depuis la connexion
             */
            static bool SelectOnRead(Socket& socket, Timeout& to);
			static SocketSet SelectOnRead(SocketSet& socketSet, Timeout& to);

            /**
             * Atteint le temps to que le socket socket soit prêt à écrire un flux
             * d'octets vers la connexion
             */
		    static bool SelectOnWrite(Socket& socket, Timeout& to);
			static SocketSet SelectOnWrite(SocketSet& socketSet, Timeout& to);

		    /**
             * Atteint le temps to que le socket socket ai reçu des données
             * hors bande
             */
		    static bool SelectOnExcept(Socket& socket, Timeout& to);
			static SocketSet SelectOnExcept(SocketSet& socketSet, Timeout& to);

	    private:

            /**
             * Fonction bas niveau pour la gestion des événements asynchrones
             */
		    static bool LowLevelSelect(Socket& socket, const SelectType& type, Timeout& to);
			static SocketSet LowLevelSelect(SocketSet& socketSet, const SelectType& type, Timeout& to);
    };
} // Network
