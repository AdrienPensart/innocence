#pragma once

#include <network/TcpClient.hpp>
#include <fstream>

#ifdef WIN32
	#define OTL_ODBC
	#define OTL_ODBC_SELECT_STM_EXECUTE_BEFORE_DESCRIBE
	#include <common/WindowsWrapper.hpp>
	#include <sql.h>
	#include <sqlext.h>
	#include <otlv4.h>
#endif

namespace Log
{
	class Message;

	class LogObserver
	{
		public:

			virtual ~LogObserver();
			virtual void update(const Message& message)=0;
	};

	class LogToFile : public LogObserver
	{
		public:

			LogToFile(const std::string& filepath);
			virtual void update(const Message& message);

		private:

			bool disabled;
			const std::string filepath;
			std::ofstream file;
	};

	class LogToNetwork : public LogObserver
	{
		public:

			LogToNetwork(const Network::Host& loggerIp, const Network::Port& loggerPort);
			virtual void update(const Message& message);
			Network::Stream& getStream();

		private:

			bool connected;
			Network::TcpClient socket;
	};

	class LogToCollector : public LogToNetwork
	{
		public:
			LogToCollector(const Network::Host& loggerIp, const Network::Port& loggerPort);
			LogToCollector();
	};

	class LogToConsole : public LogObserver
	{
		public:

			LogToConsole(const std::string& title="");
			virtual void update(const Message& message);

		private:

			const std::string title;
	};

	#ifdef WIN32
	class LogToSql : public LogObserver
	{
		public:
			LogToSql(const std::string& connectionString);
			~LogToSql();
			virtual void update(const Message& message);
		private:
			otl_connect db;
			std::string connectionString;
			otl_stream * logdb;
	};
	#endif
} // Log
