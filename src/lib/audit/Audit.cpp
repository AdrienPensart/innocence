#include "Audit.hpp"
#include <common/Log.hpp>

namespace Audit
{
	Run::Run(std::string moduleArg, unsigned int buildIdArg, std::string buildDateArg) :
		module(moduleArg),
		buildId(buildIdArg),
		buildDate(buildDateArg),
		startedAt(Common::genTime())
	{
	}

	Run::~Run()
	{
	}

	void Run::addMessage(const Common::Message& message)
	{
		messages.push_back(message);
	}

	std::string Run::build()
	{
		return "not implemented";
	}

	LogToAuditor::LogToAuditor(Run& runArg) : 
		run(runArg)
	{
	}

    void LogToAuditor::update(const Common::Message& message)
	{
		run.addMessage(message);
	}

	GlobalAudit::GlobalAudit()
	{
	}

	GlobalAudit::~GlobalAudit()
	{
	}

	std::string GlobalAudit::build()
	{
		return "not implemented";
	}
} // Audit
