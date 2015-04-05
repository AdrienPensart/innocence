#pragma once

#include <QThread>
#include <log/LogServer.hpp>
#include <log/Message.hpp>
using Log::Message;

namespace LogGui {

	class LogThread : public QThread {
			Q_OBJECT

		public:

			LogThread();
			~LogThread();
			void run();
			void addMessage(Message message);

		signals:

			void newMessage(Message message);

		private:

			Log::LogServer logserver;
	};

	class LogToGui : public Log::LogObserver {
		public:

			LogToGui(LogThread * logThread);
			virtual void update(const Message& message);

		private:

			LogThread * logThread;
	};
} // LogGui
