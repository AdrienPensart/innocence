#pragma once

#include "GlobalAudit.hpp"

namespace Audit {
	class DispatchAudit : public Log::LogObserver {
		public:

			DispatchAudit(GlobalAudit& globalAudit);
			virtual void update(const Log::Message& message);

		private:

			GlobalAudit& globalAudit;
	};

	class AuditServer : public Common::Singleton<AuditServer>, public System::Thread {
			friend class Common::Singleton<AuditServer>;

		public:

			virtual void start();
			virtual void stop();

			static void run();
			static void setLogServer(Log::LogServer * logServer);

		protected:

			AuditServer();

		private:

			static DWORD WINAPI LogLoop(LPVOID lpParameter);
			static Log::LogServer * logServer;
	};
} // Audit
