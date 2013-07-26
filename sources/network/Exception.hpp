#ifndef _SOCKETEXCEPT_
#define _SOCKETEXCEPT_

#include <string>
#include <sstream>
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
		public:

			Exception(const std::string msg="Unspecified Network Exception");
			virtual ~Exception() throw();
			virtual const char * what() const throw ();
			template<typename T>
			static std::string toString( const T & Value )
			{
				std::ostringstream oss;
				oss << Value;
				return oss.str();
			}

        protected:

			void setMessage(const std::string msg);

		private:

			std::string msg;
    };

} // Network

#endif // _SOCKETEXCEPT_
