#include "ServerAbstractFunction.hpp"

namespace Inhibition
{

    void ServerAbstractFunction::setSession(Blaspheme::Session& _session)
    {
        p_session = &_session;
    }
    
    ServerAbstractFunction::~ServerAbstractFunction()
    {
        
    }
    
    Blaspheme::Session * ServerAbstractFunction::p_session = 0;

} /* Inhibition */
