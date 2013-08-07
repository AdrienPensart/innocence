#pragma once

#include <string>
#include <common/Log.hpp>

class Innocence::Identity
{
	public:

		Identity
			(
				const std::string& moduleArg="", 
				const std::string& buildIdArg="", 
				const std::string buildDateArg="", 
				const std::string& buildTimeArg="", 
				const std::string& buildTimestampArg=""
			) : 
			module(moduleArg),
			buildId(buildIdArg),
			buildDate(buildDateArg),
			buildTime(buildTimeArg),
			buildTimestamp(buildTimestampArg)
		{
			//LOG.setIdentity(*this);
		}

		const std::string& getModule() const
		{
			return module;
		}

		const std::string& getBuildId() const
		{
			return buildId;
		}

		const std::string& getBuildDate() const
		{
			return buildDate;
		}

		const std::string& getBuildTime() const
		{
			return buildTime;
		}

		const std::string& getBuildTimestamp() const
		{
			return buildTimestamp;
		}

	private:

		std::string module;
		std::string buildId;
		std::string buildDate;
		std::string buildTime;
		std::string buildTimestamp;
};
