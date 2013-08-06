#include <common/Log.hpp>
using namespace Common;

#include <system/Uac.hpp>
using namespace System;

#include <Innocence.hpp>

int main(int argc, char *argv[])
{
	LOG.setHeader(Innocence::ISADMIN_AUDIT_HEADER);
	LOG.addObserver(new Common::LogToCollector);
	LOG.addObserver(new Common::LogToConsole);
	if(isAdministrator())
	{
		LOG << "You are administrator !";
		return EXIT_SUCCESS;
	}
	LOG << "You are NOT administrator";
	return EXIT_FAILURE;
}
