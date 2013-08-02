#pragma once

#include <string>
#include <Innocence.hpp>

namespace Blaspheme
{
    class Session;

    class AuthenticationMethod
    {
        public:

			AuthenticationMethod(const Innocence::ConnectionInfo& info);
            virtual bool sendAuth(Session&)=0;
			virtual bool recvAuth(Session&)=0;
			const Innocence::ConnectionInfo& getInfo();

		private:

			const Innocence::ConnectionInfo& info;
    };
    
    class NoAuthentication : public AuthenticationMethod
    {
        public:

			NoAuthentication(const Innocence::ConnectionInfo& info);
			virtual bool sendAuth(Session&);
			virtual bool recvAuth(Session&);
    };

    class StringBasedAuth : public AuthenticationMethod
    {
        public:

            StringBasedAuth(const Innocence::ConnectionInfo& info);
            virtual bool sendAuth(Session&);
			virtual bool recvAuth(Session&);
    };

	class ChallengedBasedAuth : public AuthenticationMethod
	{
		public:
			
			ChallengedBasedAuth(const Innocence::ConnectionInfo& info);
            virtual bool sendAuth(Session&);
			virtual bool recvAuth(Session&);
	};

} // Blaspheme
