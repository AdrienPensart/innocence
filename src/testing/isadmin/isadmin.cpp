#include <log/Log.hpp>
using namespace Common;

#include <system/Uac.hpp>
using namespace System;

#include <common/Innocence.hpp>

int main(int argc, char *argv[])
{
	TRACE_FUNCTION
	LOG.setIdentity(Common::identity);
	LOG.addObserver(new Log::LogToCollector);
	LOG.addObserver(new Log::LogToConsole);
	if(isAdministrator())
	{
		LOG << "You are administrator !";
		return EXIT_SUCCESS;
	}
	LOG << "You are NOT administrator";
	return EXIT_FAILURE;
}
