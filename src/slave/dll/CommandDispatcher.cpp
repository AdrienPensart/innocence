#include "CommandDispatcher.hpp"

namespace Inhibition
{

    CommandDispatch::~CommandDispatch()
    {
        for(Functions::iterator iter_func = functions.begin();
            iter_func != functions.end();
            iter_func++)
        {
            delete iter_func->second;
        }
    }

    void CommandDispatch::addServerFunction(const std::string& name, ServerAbstractFunction * pFunc)
    {
        functions.insert(Functions::value_type(name, pFunc));
    }

    bool CommandDispatch::dispatch(const std::string& inputCmd)
    {    
        Functions::iterator function_to_execute = functions.find(inputCmd);
        if(function_to_execute != functions.end())
        {
            (*(function_to_execute->second))();
            return true;
        }
        return false;
    }

    bool CommandDispatch::find(const std::string& inputCmd)
    {
        return functions.find(inputCmd) != functions.end();
    }
    
}

