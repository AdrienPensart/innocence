#pragma once

#include <log/LogObserver.hpp>
#include <log/Message.hpp>
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
} // Audit
