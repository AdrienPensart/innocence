#include <log/Log.hpp>
#include <system/Uac.hpp>
#include <common/Innocence.hpp>
#include <audit/Audit.hpp>

int main(int argc, char *argv[])
{
	TRACE_FUNCTION
	LOG.setIdentity(Common::identity);
	LOG.addObserver(new Log::LogToConsole);
	LOG.addObserver(new Log::LogToCollector);
	LOG.addObserver(new Audit::LogToAuditor);

	if(System::isAdministrator())
	{
		LOG << "You are administrator !";
		return EXIT_SUCCESS;
	}
	LOG << "You are NOT administrator";
	return EXIT_FAILURE;
}
