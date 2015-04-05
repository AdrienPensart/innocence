#include "LogThread.hpp"

#include <common/Innocence.hpp>
#include <log/Log.hpp>

namespace LogGui {
	LogToGui::LogToGui(LogThread * logThreadArg) :
		logThread(logThreadArg) {
	}

	void LogToGui::update(const Log::Message& message) {
		Log::Message msg = message;
		logThread->addMessage(msg);
	}

	LogThread::LogThread() :
		logserver(Common::LOG_COLLECTOR_PORT) {
		logserver.addObserver(new Log::LogToConsole);
		logserver.addObserver(new LogToGui(this));
	}

	LogThread::~LogThread() {
		logserver.stop();
		wait();
	}

	void LogThread::addMessage(Log::Message message) {
		emit newMessage(message);
	}

	void LogThread::run() {
		logserver.run();
	}
} // LogGui
