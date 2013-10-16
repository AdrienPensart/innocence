#include "Registry.hpp"
#include <common/WindowsWrapper.hpp>
#include <winerror.h>
#include <tchar.h>
#include <strsafe.h>

#include <log/Log.hpp>

namespace System
{
	namespace Registry
	{
		std::string GenErrorMessage(const std::string& msg, long code)
		{
			std::string def = msg;
			if(code != -1)
			{
				const int buffSize = 1024;
				char* buff = new char[buffSize];
				FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,0,code,0,buff,buffSize,0);
				def += " : ";
				def += std::string(buff, buffSize);
			}
			return def;
		}

		RegistryError::RegistryError(const std::string& msg, long code)
		{
			std::string buffer = "RegistryError : " + msg;
			if(code)
			{
				buffer += " (" + GenErrorMessage(msg, code) + ")";
			}
			LOG << buffer;
			setMessage(buffer);
		}

		// code extrait de la MSDN
		// efface récursivement une clé de registre et toutes ses sous-clés
		bool RegDelnodeRecurse (HKEY hKeyRoot, LPTSTR lpSubKey)
		{
			LPTSTR lpEnd;
			LONG lResult;
			DWORD dwSize;
			TCHAR szName[MAX_PATH];
			HKEY hKey;
			FILETIME ftWrite;
			lResult = RegDeleteKey(hKeyRoot, lpSubKey);

			if (lResult == ERROR_SUCCESS) 
			{
				return true;
			}

			lResult = RegOpenKeyEx (hKeyRoot, lpSubKey, 0, KEY_READ, &hKey);
			if (lResult != ERROR_SUCCESS || lResult != ERROR_FILE_NOT_FOUND) 
			{
				return false;
			}

			lpEnd = lpSubKey + lstrlen(lpSubKey);
			if (*(lpEnd - 1) != TEXT('\\')) 
			{
				*lpEnd =  TEXT('\\');
				lpEnd++;
				*lpEnd =  TEXT('\0');
			}
			dwSize = MAX_PATH;
			lResult = RegEnumKeyEx(hKey, 0, szName, &dwSize, NULL, NULL, NULL, &ftWrite);
			if (lResult == ERROR_SUCCESS) 
			{
				do
				{
					StringCchCopy (lpEnd, MAX_PATH*2, szName);
					if (!RegDelnodeRecurse(hKeyRoot, lpSubKey))
					{
						break;
					}

					dwSize = MAX_PATH;
					lResult = RegEnumKeyEx(hKey, 0, szName, &dwSize, NULL, NULL, NULL, &ftWrite);
				} while (lResult == ERROR_SUCCESS);
			}
			lpEnd--;
			*lpEnd = TEXT('\0');
			RegCloseKey (hKey);
			lResult = RegDeleteKey(hKeyRoot, lpSubKey);

			if (lResult == ERROR_SUCCESS) 
			{
				return true;
			}
			return false;
		}

		const int MAX_STRING_SIZE = 255;
		const int CLASS_NAME_LENGTH = 255;

		HKEY translateBase(const RegistryBase& base)
		{
			switch(base)
			{
				case classesRoot:
					return HKEY_CLASSES_ROOT;
				case currentUser:
					return HKEY_CURRENT_USER;
				case localMachine:
					return HKEY_LOCAL_MACHINE;
				case currentConfig:
					return HKEY_CURRENT_CONFIG;
				case users:
					return HKEY_USERS;
				case perfData:
					return HKEY_PERFORMANCE_DATA;
				case perfNlsText:
					return HKEY_PERFORMANCE_NLSTEXT;
				case perfText:
					return HKEY_PERFORMANCE_TEXT;
				default:
					// la base donnée en paramètre ne correspond à aucune base de Microsoft
					throw RegistryError("Bad registry database");
			}
			// n'atteint jamais ce point
			return HKEY();
		}

		Key::Key(const RegistryBase& _base, const std::string& _path, bool doCreation)
			:base(_base), path(_path)
		{
			HKEY root = translateBase(base);
			HKEY tryOpenkey;
			// essai d'ouvrir la clé
			LONG lResult = RegOpenKeyEx(root, path.c_str(), 0, KEY_ALL_ACCESS, &tryOpenkey);
			if (lResult == ERROR_SUCCESS)
			{
				// la clé existe, on la referme
				RegCloseKey(tryOpenkey);
			}
			else
			{
				// la clé n'existe pas
				if(doCreation)
				{
					HKEY createKey;
					DWORD disposition = 0;
					LONG result = RegCreateKeyEx(root, path.c_str(), 0, NULL,REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &createKey, &disposition);
					if (result == ERROR_SUCCESS)
					{
						// la création a fonctionnée
						RegCloseKey(createKey);
					}
					else
					{
						// la création a échouée
						throw RegistryError("Can't create key", result);
					}
				}
				else
				{
					throw RegistryError("Key does not exist", lResult);
				}
			}
		}

		bool Key::exists()
		{
			HKEY root = translateBase(base);
			HKEY hKey;
			LONG result = RegOpenKeyEx(root, path.c_str(), 0, KEY_ALL_ACCESS, &hKey);
			RegCloseKey(hKey);
			if (result == ERROR_SUCCESS)
			{
				return true;
			}
			return false;
		}

		bool Key::create()
		{
			HKEY root = translateBase(base);
			HKEY key;
			LONG result = RegOpenKeyEx(root, path.c_str(), 0, KEY_ALL_ACCESS, &key);
			if (result == ERROR_SUCCESS)
			{
				// la clé existe déjà
				RegCloseKey(key);
				return true;
			}
			
			// création de la clé
			DWORD disposition = 0;
			result = RegCreateKeyEx(root, path.c_str(), 0, NULL,REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key, &disposition);
			if(result == ERROR_SUCCESS)
			{
				RegCloseKey(key);
				return true;
			}
			return false;
		}

		bool Key::remove()
		{
			// la destruction d'une clé se fait récursivement
			// si la clé n'existe pas, on ne fait rien
			if (!exists())
			{
				return true;
			}
			
			HKEY root = translateBase(base);
			return RegDelnodeRecurse(root, (LPTSTR)path.c_str());
		}

		Key Key::createSubkey(const std::string& subkeyName)
		{
			return Key(base, path+"\\"+subkeyName, true);
		}

		bool Key::createValue(const std::string& valueName, const std::string& value)
		{
			BOOL bSuccess = TRUE;
			HKEY key;
			HKEY root = translateBase(base);
			
			if (value.size() > MAX_STRING_SIZE-1)
			{
				return false;
			}

			if (RegOpenKeyEx(root, path.c_str(), 0,
				KEY_WRITE, &key) != ERROR_SUCCESS)
			{
				return false;
			}

			if (RegSetValueEx(key, valueName.c_str(), 0,
				REG_SZ, (BYTE *)value.c_str(), value.size() + 1)
				 != ERROR_SUCCESS)
			{
				RegCloseKey(key);
				return false;
			}
			RegCloseKey(key);
			return true;
		}

		bool Key::removeValue(const std::string& valueName)
		{
			HKEY key;
			HKEY root = translateBase(base);
			if (RegOpenKeyEx(root, path.c_str(), 0,
				KEY_SET_VALUE, &key) != ERROR_SUCCESS)
			{
				return false;
			}
			LONG result = RegDeleteValue(key, valueName.c_str());
			RegCloseKey(key);
			if (result == ERROR_SUCCESS)
			{
				return true;
			}
			return false;
		}

		bool Key::isValueExists(const std::string& valueName)
		{
			HKEY key;
			HKEY root = translateBase(base);
			LONG result = RegOpenKeyEx(root, path.c_str(), 0, KEY_ALL_ACCESS, &key);
			if(result != ERROR_SUCCESS)
			{
				return false;
			}

			result = RegQueryValueEx(key, valueName.c_str(), NULL, NULL, NULL, NULL);
			RegCloseKey(key);
			if (result == ERROR_SUCCESS)
			{
				// la valeur existe
				return true;
			}
			return false;
		}

		bool Key::setValue(const std::string& valueName, const std::string& value)
		{
			HKEY key;
			HKEY root = translateBase(base);
			if (RegOpenKeyEx(root, path.c_str(), 0, KEY_WRITE, &key) != ERROR_SUCCESS)
			{
				// la clé n'existe pas
				return false;
			}
			bool success = true;
			if (RegSetValueEx(key, valueName.c_str(), 0, REG_SZ, (const BYTE *)value.c_str(), value.size()) != ERROR_SUCCESS)
			{
				success = false;
			}
			RegCloseKey(key);
			return success;
		}

		bool Key::getValue(const std::string& valueName, std::string& value)
		{
			HKEY key;
			HKEY root = translateBase(base);

			LONG result = RegOpenKeyEx(root, path.c_str(), 0,KEY_QUERY_VALUE, &key);
			if (result != ERROR_SUCCESS)
			{
				// la clé n'existe pas
				return false;
			}

			// récupération du type de la valeur
			DWORD type = 1;
			result = RegQueryValueEx(key, valueName.c_str(), NULL, &type, NULL, NULL);
			if (result != ERROR_SUCCESS)
			{
				// si on arrive pas a récupérer le type de la valeur
				RegCloseKey(key);
				return false;
			}

			// être sur que c'est une valeur de type chaine
			if (type != REG_SZ && type != REG_EXPAND_SZ)
			{
				RegCloseKey(key);
				return false;
			}

			DWORD dwSize = 255;
			char sz[255];
			result = RegQueryValueEx(key, valueName.c_str(), NULL, &type, (LPBYTE)sz, &dwSize);
			if(result != ERROR_SUCCESS)
			{
				RegCloseKey(key);
				return false;
			}
			RegCloseKey(key);
			value = (LPCTSTR)sz;
			return true;
		}
		
		RegistryType Key::getValueType(const std::string& valueName)
		{
			HKEY key;
			HKEY root = translateBase(base);
			LONG result = RegOpenKeyEx(root, path.c_str(), 0, KEY_QUERY_VALUE, &key);
			if (result != ERROR_SUCCESS)
			{
				throw RegistryError("Key does not exist", result);
			}

			DWORD dwType = 1;
			result = RegQueryValueEx(key, valueName.c_str(), NULL, &dwType, NULL, NULL);
			RegCloseKey(key);
			if (result == ERROR_SUCCESS)
			{
				switch(dwType)
				{
					case REG_BINARY:
						return TYPE_BINARY;
					case REG_DWORD:
						return TYPE_DWORD;
					case REG_EXPAND_SZ:
						return TYPE_EXPAND_SZ;
					case REG_MULTI_SZ:
						return TYPE_MULTI_SZ;
					case REG_SZ:
						return TYPE_SZ;
					case REG_FULL_RESOURCE_DESCRIPTOR:
						return TYPE_FULL_RESOURCE_DESCRIPTOR;
					case REG_LINK:
						return TYPE_LINK;
					case REG_QWORD:
						return TYPE_QWORD;
					case REG_RESOURCE_LIST:
						return TYPE_RESOURCE_LIST;
					case REG_RESOURCE_REQUIREMENTS_LIST:
						return TYPE_RESOURCE_REQUIREMENTS_LIST;
				}
			}
			return TYPE_NONE;
		}
		
		Values Key::getValues()
		{
			Values values;
			return values;
		}

		KeyList Key::getKeys()
		{
			KeyList list;
			return list;
		}

		int Key::subKeysCount()
		{
			HKEY key;
			HKEY root = translateBase(base);
			if (RegOpenKeyEx(root, path.c_str(), 0,
				KEY_ALL_ACCESS, &key) != ERROR_SUCCESS)
			{
				return -1;
			}

			
			DWORD dwSubKeyCount, dwValueCount, dwClassNameLength,
				dwMaxSubKeyName, dwMaxValueName, dwMaxValueLength;
			FILETIME ftLastWritten;

			_TCHAR szClassBuffer[CLASS_NAME_LENGTH];
			dwClassNameLength = CLASS_NAME_LENGTH;

			LONG result = RegQueryInfoKey(key, szClassBuffer, &dwClassNameLength,
				NULL, &dwSubKeyCount, &dwMaxSubKeyName, NULL, &dwValueCount,
				&dwMaxValueName, &dwMaxValueLength, NULL, &ftLastWritten);

			RegCloseKey(key);
			if (result != ERROR_SUCCESS)
			{
				return -1;
			}
			return (int)dwSubKeyCount;
		}

		int Key::valuesCount()
		{
			// Call this function to determine the number of subkeys.
			// the function returns -1 on error
			HKEY key;
			HKEY root = translateBase(base);

			if (RegOpenKeyEx(root, path.c_str(), 0,
				KEY_ALL_ACCESS, &key) != ERROR_SUCCESS)
			{
				return -1;
			}
			DWORD dwSubKeyCount;
			DWORD dwValueCount;
			DWORD dwClassNameLength = CLASS_NAME_LENGTH;
			DWORD dwMaxSubKeyName;
			DWORD dwMaxValueName;
			DWORD dwMaxValueLength;
			FILETIME ftLastWritten;
			_TCHAR szClassBuffer[CLASS_NAME_LENGTH];

			LONG result = RegQueryInfoKey(key, szClassBuffer, &dwClassNameLength,
				NULL, &dwSubKeyCount, &dwMaxSubKeyName, NULL, &dwValueCount,
				&dwMaxValueName, &dwMaxValueLength, NULL, &ftLastWritten);
			RegCloseKey(key);
			if (result != ERROR_SUCCESS)
			{
				return -1;
			}
			return (int)dwValueCount;
		}

	} // Registry

} //System
