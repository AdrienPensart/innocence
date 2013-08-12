#pragma once

#include <string>

#include <system/System.hpp>

#include "Utility.hpp"
#include "Serializable.hpp"

namespace Common
{
	namespace Private
	{
		static const char * marker = "I#I";
	}

	class Identity : public Serializable
	{
		public:

			Identity
				(
					const std::string& moduleArg="", 
					const std::string& buildIdArg="", 
					const std::string& buildDateArg="", 
					const std::string& buildTimeArg="", 
					const std::string& buildTimestampArg=""
				) : 
				module(moduleArg),
				buildId(buildIdArg),
				buildDate(buildDateArg),
				buildTime(buildTimeArg),
				buildTimestamp(buildTimestampArg),
				executionDate(currentDate()),
				os(System::getSystemVersionString())
			{
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

			std::string getLogFile() const
			{
				return module + "-" + buildId + "-" + Common::currentTime() + ".log";
			}

			const std::string& getExecutionDate() const
			{
				return executionDate;
			}

			const std::string& getOs() const
			{
				return os;
			}

			std::string serialize() const
			{
				return module + Private::marker + os + Private::marker + buildId + Private::marker + buildDate + Private::marker + buildTime + Private::marker + buildTimestamp;
			}

			void deserialize(const std::string& buffer)
			{
				std::vector<std::string> output;
				split(buffer, Private::marker, output);
				module = output[0];
				os = output[1];
				buildId = output[2];
				buildDate = output[3];
				buildTime = output[4];
				buildTimestamp = output[5];
			}

		private:

			std::string os;
			std::string module;
			std::string executionDate;
			std::string buildId;
			std::string buildDate;
			std::string buildTime;
			std::string buildTimestamp;
	};
} // Common
