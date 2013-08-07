#include "LogThread.hpp"

#include <Innocence.hpp>
#include <common/Log.hpp>

LogToGui::LogToGui(LogThread * logThreadArg) : 
	logThread(logThreadArg)
{
}

void LogToGui::update(const Common::Message& message)
{
	Common::Message msg = message;
	logThread->addMessage(msg);
}

LogThread::LogThread() : 
	logserver(Innocence::LOG_COLLECTOR_PORT)
{
	logserver.addObserver(new Common::LogToConsole);
	//logserver.addObserver(new LogToGui(this));
}

LogThread::~LogThread()
{
	logserver.stop();
	wait();
}

void LogThread::addMessage(Common::Message message)
{
	emit newMessage(message);
}

void LogThread::run()
{
	logserver.run();
}
