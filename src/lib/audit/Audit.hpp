#pragma once

#include <common/LogObserver.hpp>

#include <common/Message.hpp>
#include <vector>

namespace Audit
{
	class Run
	{
		public:
			
			Run(std::string module, unsigned int buildId, std::string buildDate);
			virtual ~Run();
			void addMessage(const Common::Message& message);
			virtual std::string build();

		private:

			const unsigned int buildId;
			const std::string buildDate;
			const std::string module;
			std::vector<Common::Message> messages;
			const std::string startedAt;
	};

	class LogToAuditor : public Common::LogObserver
	{
		public:

            LogToAuditor(Run& run);
            virtual void update(const Common::Message& message);

        private:

            Run& run;
	};

	class GlobalAudit
	{
		public:
			
			GlobalAudit();
			virtual ~GlobalAudit();
			virtual std::string build();

		private:

			std::vector<Run> audits;
			time_t startedAt;
	};
} // Audit
