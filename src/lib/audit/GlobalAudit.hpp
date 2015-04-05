#pragma once

#include <common/Singleton.hpp>
#include <log/LogServer.hpp>
#include <log/LogObserver.hpp>
#include <log/Message.hpp>
#include <system/Thread.hpp>
#include <vector>

namespace Audit {
	class Run;

	class GlobalAudit {
		public:

			GlobalAudit();
			virtual ~GlobalAudit();
			void addRun(Run *run);
			void addMessage(const Log::Message& msg);
			void run();
			void build();

		private:

			std::vector<Log::Message> lostMessages;
			std::vector<Run*> audits;
			time_t startedAt;
	};
} // Audit
