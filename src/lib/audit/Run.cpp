#include "Run.hpp"

#include <common/Utility.hpp>
#include <system/Process.hpp>
#include <system/File.hpp>
#include <log/Log.hpp>

namespace Audit
{
	Run::Run(const std::string& executableArg) :
		executable(executableArg),
		result(EXIT_FAILURE),
		startedAt(Common::currentDate())
	{
		module = System::GetFileName(executable);
	}

	void Run::run()
	{
		System::Process::Launcher auditExeProcess(executable);
		result = auditExeProcess.wait();
	}

	void Run::build()
	{
		for(std::vector<Log::Message>::size_type index = 0; index != messages.size(); index++)
		{
			const Log::Message& message = messages[index];
			LOG << message.getIdentity().getModule() 
				   + " -> (line " << message.getLine() 
				   + " in " << System::GetFileName(message.getFile()) 
				   + ") (" << message.getCallStack() 
				   + ") : " 
				   + message.getContent() 
				   + '\n';
		}

		if(result == EXIT_SUCCESS)
		{
			LOG << "Test of " + executable + " passed => system vulnerable";
		}
		else
		{
			LOG << "Test of " + executable + " failed => system immuned";
		}
	}

	const std::string& Run::getModule()
	{
		return module;
	}

	void Run::addMessage(const Log::Message& message)
	{
		messages.push_back(message);
	}
} // Audit
