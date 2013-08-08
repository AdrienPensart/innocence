#include <log/Log.hpp>
#include <malicious/Screenshot.hpp>
using namespace Malicious;

#include <common/Innocence.hpp>

int main(int argc, char * argv[])
{
	LOG.setIdentity(Common::identity);
	LOG.addObserver(new Log::LogToNetwork("127.0.0.1", 80));

	Screenshot sc;
	sc.take("test.jpg", 100);
	return EXIT_SUCCESS;
}
