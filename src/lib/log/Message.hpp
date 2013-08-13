#pragma once

#include <common/Serializable.hpp>
#include <common/Identity.hpp>
#include <string>

namespace Log
{
	class Message : public Common::Serializable
	{
		public:
			
			Message();
			Message(const Common::Identity& identity, const std::string& content, const std::string& callStack, const std::string& line, const std::string& file);
			
			const Common::Identity& getIdentity() const;
			Common::Identity& getIdentity();
			const std::string& getContent() const ;
			const std::string& getCallStack() const ;
			const std::string& getTime() const ;
			const std::string& getLine() const ;
			const std::string& getFile() const ;

			std::string serialize() const;
			void deserialize(const std::string& buffer);

		private:

			Common::Identity identity;
			std::string content;
			std::string callStack;
			std::string emittedTime;
			std::string line;
			std::string file;
	};
} // Log
