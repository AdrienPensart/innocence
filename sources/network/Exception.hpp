#ifndef _SOCKETEXCEPT_
#define _SOCKETEXCEPT_

#include <sstream>
#include <string>
#include <exception>

namespace Network
{

/**
 * Classes d'erreur pour les communications socket, elles signalent des
 * erreurs irrécupérables si un changement d'option n'est pas effectué.
 * Exemples : retour d'une primitive socket signifiant une erreur, du style
 * bind(): cannot reuse addresses!
 */
class Exception : public std::exception
{
    protected:
        /**
         * Fonction utilitaire de conversion d'un entier en string
         */
        virtual ~Exception()throw(){}
        static std::string intToString(const unsigned int& value)
        {
            std::ostringstream oss;
            oss << value;
            return oss.str();
        }
};

} /* Network */

#endif // _SOCKETEXCEPT_
