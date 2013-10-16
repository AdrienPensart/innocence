#pragma once

#include <log/Message.hpp>
#include <string>
#include <vector>
#include <common/WindowsWrapper.hpp>

namespace Audit
{
	class Run
	{
		public:
			
			Run(const std::string& executable);
			void run();
			void build();
			void addMessage(const Log::Message& message);
			const std::string& getModule();

		private:

			DWORD result;
			std::string executable;
			std::string module;
			std::string startedAt;
			std::vector<Log::Message> messages;
	};

} // Audit
