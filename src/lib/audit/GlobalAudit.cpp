#include "GlobalAudit.hpp"
#include "Run.hpp"

namespace Audit
{
	GlobalAudit::GlobalAudit()
	{
	}

	GlobalAudit::~GlobalAudit()
	{
		for(std::vector<Run*>::size_type index = 0; index != audits.size(); index++)
		{
			delete audits[index];
		}
	}

	void GlobalAudit::addRun(Run * run)
	{
		audits.push_back(run);
	}

	void GlobalAudit::addMessage(const Log::Message& msg)
	{
		for(std::vector<Run*>::size_type index = 0; index != audits.size(); index++)
		{
			if(audits[index]->getModule() == msg.getIdentity().getModule())
			{
				audits[index]->addMessage(msg);
				return;
			}
		}
		lostMessages.push_back(msg);
	}

	void GlobalAudit::run()
	{
		for(std::vector<Run*>::size_type index = 0; index != audits.size(); index++)
		{
			audits[index]->run();
		}
	}

	void GlobalAudit::build()
	{
		for(std::vector<Run*>::size_type index = 0; index != audits.size(); index++)
		{
			audits[index]->build();
		}


	}
} // Audit
