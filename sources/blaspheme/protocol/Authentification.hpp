#ifndef AUTHENTIFICATION_HPP
#define AUTHENTIFICATION_HPP

#include <string>

namespace Blaspheme
{
    class Session;

    class AuthentificationMethod
    {
        public:

            virtual bool sendAuth(Session&)=0;
			virtual bool recvAuth(Session&)=0;
    };
    
    class NoAuthentification : public AuthentificationMethod
    {
        public:

			virtual bool sendAuth(Session&){return true;}
			virtual bool recvAuth(Session&){return true;}
    };

    class StringBasedAuth : public AuthentificationMethod
    {
        public:

            StringBasedAuth(const std::string& string_password);
            void setPassword(const std::string& string_password);
            virtual bool sendAuth(Session&);
			virtual bool recvAuth(Session&);

        protected:

            std::string password;
    };

} /* Blaspheme */

#endif // AUTHENTIFICATION_HPP
