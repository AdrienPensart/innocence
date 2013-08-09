#include <log/Log.hpp>
using namespace Common;

#include <system/Process.hpp>
#include <common/Innocence.hpp>

#include <malicious/Elevator.hpp>

#include <audit/Audit.hpp>

int main(int argc, char *argv[])
{
	try
	{
		LOG.setIdentity(Common::identity);
		LOG.addObserver(new Log::LogToConsole);
		LOG.addObserver(new Log::LogToCollector);
		LOG.addObserver(new Audit::LogToAuditor);

		System::Process::This thisProcess;
		return Malicious::elevate(thisProcess.getProgramDir()+"\\isadmin.exe");
    }
    catch(Common::Exception&)
    {
    }
	CATCH_UNKNOWN_EXCEPTION
	return EXIT_FAILURE;
}
