#include <common/Log.hpp>
using namespace Common;

#include <system/Uac.hpp>
using namespace System;

#include <auditor/Auditor.hpp>

int main(int argc, char *argv[])
{
	LOG.addObserver(new Common::LogToNetwork("127.0.0.1", 80));
	LOG.addObserver(new Common::LogToConsole);
	if(isAdministrator())
	{
		LOG.setHeader(ISADMIN_AUDIT_HEADER);
		LOG << "You are administrator !";
		return EXIT_SUCCESS;
	}
	
	LOG.setHeader("TEST ELEVATION (normal user)");
	LOG << "You are NOT administrator";
	return EXIT_FAILURE;
}
