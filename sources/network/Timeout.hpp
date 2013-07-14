#ifndef TIMEOUT_H
#define TIMEOUT_H

#include "Types.hpp"

namespace Network
{
    /**
     * Représente un timeout, soit d'une durée déterminée (limite finie)
     * ou bien indéterminée (attendre indéfiniment).
     * Cette classe pourra servir à spécifier des timeouts lors 
     * des envois / réceptions.
     */
    class Timeout
    {
        public:
			/**
			 * Constructeur, par défaut, le timeout est toujours nul, c'est à dire un appel non bloquant.
			 */
            Timeout(const long& sec=0, const long& usec=0);

			/**
			 * Paramètre le timeout avec une durée en seconde et microsecondes.
			 */
            void set(const long& sec, const long& usec=0);
			
			/**
			 * Renvoi la structure C sous-jacente afin de garder une interface
			 * avec la fonction select() par exemple.
			 */
            timeval & c_struct();
		private:
			timeval time;
	};

} /* Network */

#endif // TIMEOUT_H
