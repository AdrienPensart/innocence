#pragma once

#include <common/Serializable.hpp>
#include <common/Identity.hpp>
#include <string>

namespace Log
{
	std::string genTime();

	class Message : public Common::Serializable
	{
		public:
			
			Message();
			Message(const Common::Identity& identity, const std::string& content, const std::string& callStack);
			
			const Common::Identity& getIdentity() const;
			const std::string& getContent() const ;
			const std::string& getCallStack() const ;
			const std::string& getTime() const ;

			std::string serialize() const;
			void deserialize(const std::string& buffer);

		private:

			static unsigned int id;
			Common::Identity identity;
			std::string content;
			std::string callStack;
			std::string emittedTime;
	};

} // Log
