#ifndef _SERVER_ABSTRACT_FUNCTION_
#define _SERVER_ABSTRACT_FUNCTION_

#include <blaspheme/protocol/Session.hpp>

namespace Inhibition
{

    // le serveur possede une liste de fonctions appelables par le client
    // certains modules sont interdependants, ils conservent donc des liens
    // vers ces modules
    class ServerAbstractFunction
    {
        public:
            virtual ~ServerAbstractFunction();
            static void setSession(Blaspheme::Session& _session);
            virtual void operator()()=0;
            Blaspheme::Session& session(){return *p_session;}
        protected:
            static Blaspheme::Session * p_session;
    };

} /* Inhibition */

#endif // _SERVER_ABSTRACT_FUNCTION_
