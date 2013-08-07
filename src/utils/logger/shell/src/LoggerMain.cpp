#include <common/LogServer.hpp>
#include <Innocence.hpp>

int main(int argc, char * argv[])
{
	Common::LogServer logserver(Innocence::LOG_COLLECTOR_PORT);
	logserver.addObserver(new Common::LogToConsole);
	logserver.run();
	return EXIT_SUCCESS;
}
