#include <common/Log.hpp>
using namespace Common;

#include <system/Process.hpp>
#include <auditor/Auditor.hpp>

#include <malicious/Elevator.hpp>
using namespace Malicious;

int main(int argc, char *argv[])
{
	try
	{
		LOG.setHeader(ELEVATOR_AUDIT_HEADER);
		LOG.addObserver(new Common::LogToNetwork(AUDIT_COLLECTOR_IP, AUDIT_COLLECTOR_PORT));

		System::Process::This thisProcess;

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
