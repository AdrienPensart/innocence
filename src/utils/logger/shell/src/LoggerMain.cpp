#include <log/LogServer.hpp>
#include <log/Log.hpp>
#include <log/Message.hpp>
#include <common/Innocence.hpp>

int main(int argc, char * argv[])
{
	Log::LogServer logserver(Common::LOG_COLLECTOR_PORT);
	logserver.addObserver(new Log::LogToConsole);
	logserver.run();
	return EXIT_SUCCESS;
}
