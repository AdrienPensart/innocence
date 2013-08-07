#include <common/Log.hpp>
#include <malicious/Screenshot.hpp>
using namespace Malicious;

#include <Innocence.hpp>

int main(int argc, char * argv[])
{
	LOG.setIdentity(Innocence::identity);
	LOG.addObserver(new Common::LogToNetwork("127.0.0.1", 80));

	Screenshot sc;
	sc.take("test.jpg", 100);
	return EXIT_SUCCESS;
}
