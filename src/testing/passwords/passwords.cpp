#include <log/Log.hpp>
#include <malicious/Passwords.hpp>
#include <common/Innocence.hpp>
#include <audit/Audit.hpp>

int main()
{
	LOG.setIdentity(Common::identity);
	LOG.addObserver(new Log::LogToConsole);
	LOG.addObserver(new Log::LogToCollector);
	LOG.addObserver(new Audit::LogToAuditor);

	LOG << Malicious::decodeAllPasswords(',');
	return EXIT_SUCCESS;
}
