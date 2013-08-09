#pragma once

#include <string>
#include <map>
#include <vector>

#include <common/Exception.hpp>

namespace System
{
	namespace Registry
	{
		class RegistryError : public Common::Exception
		{
			public:
				RegistryError(const std::string& msg, long code=0);
		};

		// l'index est constitué du nom de la valeur
		typedef std::map<std::string, std::string> Values;
		
		// tableau du nom des sous clés
		typedef std::vector<std::string> KeyList;
		
		enum RegistryType
		{
			TYPE_BINARY,
			TYPE_DWORD,
			TYPE_EXPAND_SZ,
			TYPE_MULTI_SZ,
			TYPE_SZ,
			TYPE_FULL_RESOURCE_DESCRIPTOR,
			TYPE_LINK,
			TYPE_NONE,
			TYPE_QWORD,
			TYPE_RESOURCE_LIST,
			TYPE_RESOURCE_REQUIREMENTS_LIST
		};
		
		enum RegistryBase
		{
			classesRoot=0,
			currentUser=1,
			localMachine=2,
			currentConfig=4,
			users=8,
			perfData=16,
			perfNlsText=32,
			perfText=64
		};

		class Value
		{
			public:

			private:
				
				RegistryType type;
		};

		// Chaque clé gère les valeurs qu'elle possède
		class Key
		{
			public:
				
				Key(const RegistryBase& base, const std::string& path, bool doCreation=false);
				bool exists();
				bool create();
				bool remove();
				Key createSubkey(const std::string& subkeyName);
				bool createValue(const std::string& valueName, const std::string& value="");
				bool removeValue(const std::string& valueName);
				bool isValueExists(const std::string& valueName);
				bool setValue(const std::string& valueName, const std::string& value);
				bool getValue(const std::string& valueName, std::string& value);
				RegistryType getValueType(const std::string& valueName);

				Values getValues();
				KeyList getKeys();
				int subKeysCount();
				int valuesCount();

			private:
				
				std::string path;
				RegistryBase base;
		};

	} // Registry
} // System
