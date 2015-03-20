#pragma once

#include <blaspheme/protocol/Session.hpp>

namespace Inhibition
{
	class SlaveCore;

	// le serveur possede une liste de fonctions appelables par le client
	// certains modules sont interdependants, ils conservent donc des liens
	// vers ces modules
	class SlaveAbstractFunction
	{
		public:

			virtual ~SlaveAbstractFunction();
			static void setSlave(SlaveCore& slave);
			virtual void operator()()=0;
			Blaspheme::Session& session();
			SlaveCore& slave();

		protected:

			static SlaveCore * pslave;
	};

} // Inhibition
