#ifndef AUTHENTICATION_HPP
#define AUTHENTICATION_HPP

#include <string>

namespace Blaspheme
{
    class Session;

    class AuthenticationMethod
    {
        public:

            virtual bool sendAuth(Session&)=0;
			virtual bool recvAuth(Session&)=0;
    };
    
    class NoAuthentication : public AuthenticationMethod
    {
        public:

			virtual bool sendAuth(Session&);
			virtual bool recvAuth(Session&);
    };

    class StringBasedAuth : public AuthenticationMethod
    {
        public:

            StringBasedAuth(const std::string& password="");
            void setPassword(const std::string&);
            virtual bool sendAuth(Session&);
			virtual bool recvAuth(Session&);

        protected:

            std::string password;
    };

	class ChallengedBasedAuth
	{
		public:
			

	};

} /* Blaspheme */

#endif // AUTHENTICATION_HPP
