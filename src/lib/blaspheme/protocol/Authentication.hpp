#pragma once

#include <string>
#include <common/Settings.hpp>

namespace Blaspheme
{
	class Session;

	class AuthenticationMethod
	{
		public:

			AuthenticationMethod(const Common::ConnectionInfo& info);
			virtual bool sendAuth(Session&)=0;
			virtual bool recvAuth(Session&)=0;
			const Common::ConnectionInfo& getInfo();

		private:

			const Common::ConnectionInfo& info;
	};

	class NoAuthentication : public AuthenticationMethod
	{
		public:

			NoAuthentication(const Common::ConnectionInfo& info);
			virtual bool sendAuth(Session&);
			virtual bool recvAuth(Session&);
	};

	class StringBasedAuth : public AuthenticationMethod
	{
		public:

			StringBasedAuth(const Common::ConnectionInfo& info);
			virtual bool sendAuth(Session&);
			virtual bool recvAuth(Session&);
	};

	class ChallengedBasedAuth : public AuthenticationMethod
	{
		public:

			ChallengedBasedAuth(const Common::ConnectionInfo& info);
			virtual bool sendAuth(Session&);
			virtual bool recvAuth(Session&);
	};

} // Blaspheme
