#include "CommandDispatcher.hpp"
#include <log/Log.hpp>

namespace Inhibition {

	CommandDispatch::~CommandDispatch() {
		for(Functions::iterator iter_func = functions.begin();
		        iter_func != functions.end();
		        iter_func++) {
			delete iter_func->second;
		}
	}

	void CommandDispatch::addServerFunction(const std::string& name, SlaveAbstractFunction * pFunc) {
		LOG << "Adding new function : " + name + " at address : " + Common::toString(pFunc);
		functions.insert(Functions::value_type(name, pFunc));
	}

	bool CommandDispatch::dispatch(const std::string& inputCmd) {
		Functions::iterator function_to_execute = functions.find(inputCmd);
		if(function_to_execute != functions.end()) {
			LOG << "Executing function " + inputCmd + " at address : " + Common::toString(function_to_execute->second);
			(*(function_to_execute->second))();
			return true;
		}
		return false;
	}

	bool CommandDispatch::find(const std::string& inputCmd) {
		return functions.find(inputCmd) != functions.end();
	}
}

