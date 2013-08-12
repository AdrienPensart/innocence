#pragma once

#include <string>

namespace Common
{
	class Serializable
	{
		public:
			virtual std::string serialize() const = 0;
			virtual void deserialize(const std::string& buffer) = 0;
	};
} // Common
