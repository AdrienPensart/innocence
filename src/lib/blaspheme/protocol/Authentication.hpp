#ifndef _AUTHENTICATION_
#define _AUTHENTICATION_

#include <string>
#include "ConnectionInfo.hpp"

namespace Blaspheme
{
    class Session;

    class AuthenticationMethod
    {
        public:

			AuthenticationMethod(const ConnectionInfo& info);
            virtual bool sendAuth(Session&)=0;
			virtual bool recvAuth(Session&)=0;
			const ConnectionInfo& getInfo();

		private:

			const ConnectionInfo& info;
    };
    
    class NoAuthentication : public AuthenticationMethod
    {
        public:

			NoAuthentication(const ConnectionInfo& info);
			virtual bool sendAuth(Session&);
			virtual bool recvAuth(Session&);
    };

    class StringBasedAuth : public AuthenticationMethod
    {
        public:

            StringBasedAuth(const ConnectionInfo& info);
            virtual bool sendAuth(Session&);
			virtual bool recvAuth(Session&);
    };

	class ChallengedBasedAuth : public AuthenticationMethod
	{
		public:
			
			ChallengedBasedAuth(const ConnectionInfo& info);
            virtual bool sendAuth(Session&);
			virtual bool recvAuth(Session&);
	};

} // Blaspheme

#endif // _AUTHENTICATION_
