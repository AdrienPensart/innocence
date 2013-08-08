#pragma once

#include <string>
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

			std::string serialize() const
			{
				return module + Private::marker + buildId + Private::marker + buildDate + Private::marker + buildTime + Private::marker + buildTimestamp;
			}

			void deserialize(const std::string& buffer)
			{
				std::string bufferCopy = buffer;
				std::vector<std::string> output;
				Serializable::split(bufferCopy, Private::marker, output);
				module = output[0];
				buildId = output[1];
				buildDate = output[2];
				buildTime = output[3];
				buildTimestamp = output[4];
			}

		private:

			std::string module;
			std::string buildId;
			std::string buildDate;
			std::string buildTime;
			std::string buildTimestamp;
	};
} // Common
