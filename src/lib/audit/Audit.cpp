#include "Audit.hpp"
#include <common/Innocence.hpp>
#include <log/Log.hpp>

namespace Audit
{
	Run::Run(const std::string& moduleArg, const std::string& buildIdArg, const std::string& buildDateArg) :
		module(moduleArg),
		buildId(buildIdArg),
		buildDate(buildDateArg),
		startedAt(Log::genTime())
	{
	}

	Run::~Run()
	{
	}

	const std::string& Run::getModule() const
	{
		return module;
	}

	const std::string& Run::getBuildId() const
	{
		return buildId;
	}

	const std::string& Run::getBuildDate() const
	{
		return buildDate;
	}

	void Run::addMessage(const Log::Message& message)
	{
		messages.push_back(message);
	}

	std::string Run::build()
	{
		return "not implemented";
	}

	LogToAuditor::LogToAuditor() : 
		LogToNetwork(Common::AUDIT_SERVER_IP, Common::AUDIT_SERVER_PORT)
	{
	}

    void LogToAuditor::update(const Log::Message& message)
	{
		//run.addMessage(message);
	}

	GlobalAudit::GlobalAudit()
	{
	}

	GlobalAudit::~GlobalAudit()
	{
	}

	void GlobalAudit::addRun(const Run& run)
	{
		audits.push_back(run);
	}

	std::string GlobalAudit::build()
	{
		return "not implemented";
	}
} // Audit
