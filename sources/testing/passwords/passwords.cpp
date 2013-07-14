#include <cstdlib>
#include <iostream>
using namespace std;

#include <common/Logger.hpp>
#include <malicious/Passwords.hpp>
using namespace Malicious;

int main()
{
	LOG.setHeader("TEST PASSWORDS");
	LOG.addObserver(new Common::LoggingMessage);
	LOG << decodeAllPasswords(',');
	return EXIT_SUCCESS;
}
