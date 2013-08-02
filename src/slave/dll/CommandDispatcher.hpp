#ifndef COMMANDDISPATCHER_H
#define COMMANDDISPATCHER_H

#include <map>
#include "SlaveAbstractFunction.hpp"

namespace Inhibition
{
    class CommandDispatch
    {
        typedef std::map<std::string, SlaveAbstractFunction *> Functions;

        public:
        
            ~CommandDispatch();
            void addServerFunction(const std::string& name, SlaveAbstractFunction * pFunc);
            bool dispatch(const std::string& inputCmd);
            bool find(const std::string& inputCmd);
        private:
            Functions functions;
    };
    
} /* Inhibition */

#endif // COMMANDDISPATCHER_H
