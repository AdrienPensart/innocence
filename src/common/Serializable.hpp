#pragma once

#include <string>
#include <vector>

namespace Common
{
	class Serializable
	{
		public:

			static void split(const std::string& str, const std::string& separator, std::vector<std::string>& results)
			{
				using std::string;
				size_t cursor = 0;
				size_t found = 0;
				while(found != string::npos)
				{
					found = str.find(separator, cursor);
					string elem = str.substr(cursor,found-cursor);
					results.push_back(elem);
					cursor = found + separator.size();
				}
			}

			virtual std::string serialize() const = 0;
			virtual void deserialize(const std::string& buffer) = 0;
	};
} // Common
