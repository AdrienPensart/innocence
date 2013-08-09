#pragma once

#include <common/Singleton.hpp>
#include <log/LogServer.hpp>
#include <log/LogObserver.hpp>
#include <log/Message.hpp>
#include <system/Thread.hpp>
#include <vector>

namespace Audit
{
	class Run
	{
		public:
			
			Run(const std::string& module, const std::string& buildId, const std::string& buildDate);
			virtual ~Run();
			void addMessage(const Log::Message& message);
			virtual std::string build();
			const std::string& getModule() const;
			const std::string& getBuildId() const;
			const std::string& getBuildDate() const;

		private:

			const std::string buildId;
			const std::string buildDate;
			const std::string module;
			std::vector<Log::Message> messages;
			const std::string startedAt;
	};

	class LogToAuditor : public Log::LogToNetwork
	{
		public:

            LogToAuditor();
            virtual void update(const Log::Message& message);

        private:

            //Run& run;
	};

	class GlobalAudit
	{
		public:
			
			GlobalAudit();
			virtual ~GlobalAudit();
			void addRun(const Run& run);
			virtual std::string build();

		private:

			std::vector<Run> audits;
			time_t startedAt;
	};

	class AuditServer : public Common::Singleton<AuditServer>, public System::Thread
	{
		public:

			virtual void start();
            virtual void stop();

			static void run();
			static void setLogServer(Log::LogServer * logServer);

		private:

			static DWORD WINAPI LogLoop(LPVOID lpParameter);
			static Log::LogServer * logServer;
	};

} // Audit
