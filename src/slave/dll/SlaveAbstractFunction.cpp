#include "SlaveAbstractFunction.hpp"
#include "SlaveCore.hpp"

namespace Inhibition
{

	void SlaveAbstractFunction::setSlave(SlaveCore& slaveArg)
	{
		pslave = &slaveArg;
	}

	SlaveAbstractFunction::~SlaveAbstractFunction()
	{
	}

	Blaspheme::Session& SlaveAbstractFunction::session()
	{
		return pslave->getSession();
	}

	SlaveCore& SlaveAbstractFunction::slave()
	{
		return *pslave;
	}

	SlaveCore * SlaveAbstractFunction::pslave = 0;

} // Inhibition
