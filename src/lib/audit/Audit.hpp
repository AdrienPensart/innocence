#pragma once

#include <log/LogObserver.hpp>
#include <common/Settings.hpp>

namespace Audit {
	class LogToAuditor : public Log::LogToNetwork {
		public:

			LogToAuditor(const Network::Host& loggerIp, const Network::Port& loggerPort) :
				LogToNetwork(loggerIp, loggerPort) {
			}

			LogToAuditor() :
				LogToNetwork(Common::AUDIT_SERVER_IP, Common::AUDIT_SERVER_PORT) {
			}
	};
} // Audit
