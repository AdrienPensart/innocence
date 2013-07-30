#ifndef COMMANDDISPATCHER_H
#define COMMANDDISPATCHER_H

#include <map>
#include "ServerAbstractFunction.hpp"

namespace Inhibition
{
    class CommandDispatch
    {
        typedef std::map<std::string, ServerAbstractFunction *> Functions;

        public:
        
            ~CommandDispatch();
            void addServerFunction(const std::string& name, ServerAbstractFunction * pFunc);
            bool dispatch(const std::string& inputCmd);
            bool find(const std::string& inputCmd);
        private:
            Functions functions;
    };
    
} /* Inhibition */

#endif // COMMANDDISPATCHER_H
