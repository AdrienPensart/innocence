#include "Keyboard.hpp"

#include <cstdio>

// Extended macros
 
#define INIT_PVK_TO_WCHARS(i, n) \
if((pKbd->pVkToWcharTable[i].cbSize - 2) / 2 == n) \
pVkToWchars##n = (PVK_TO_WCHARS##n)pKbd->pVkToWcharTable[i].pVkToWchars; \
 
#define SEARCH_VK_IN_CONVERSION_TABLE(n) \
i = 0; \
if(pVkToWchars##n && (mod < n)) \
{ \
do \
{ \
if(pVkToWchars##n[i].VirtualKey == virtualKey) \
{ \
if((pVkToWchars##n[i].Attributes == CAPLOK) && capsLock) { \
if(mod == shift) mod = 0; else mod = shift; } \
*outputChar = pVkToWchars##n[i].wch[mod]; \
charCount = 1; \
if(*outputChar == WCH_NONE) { charCount = 0; } \
else if(*outputChar == WCH_DEAD) \
{ \
*deadChar = pVkToWchars##n[i + 1].wch[mod]; \
charCount = 0; \
} \
break;\
} \
i++; \
} \
while(pVkToWchars##n[i].VirtualKey != 0); \
} \

namespace Keyboard
{
	typedef PKBDTABLES(CALLBACK* KbdLayerDescriptor)(VOID);

	PVK_TO_WCHARS1 pVkToWchars1 = NULL;
	PVK_TO_WCHARS2 pVkToWchars2 = NULL;
	PVK_TO_WCHARS3 pVkToWchars3 = NULL;
	PVK_TO_WCHARS4 pVkToWchars4 = NULL;
	PVK_TO_WCHARS5 pVkToWchars5 = NULL;
	PVK_TO_WCHARS6 pVkToWchars6 = NULL;
	PVK_TO_WCHARS7 pVkToWchars7 = NULL;
	PVK_TO_WCHARS8 pVkToWchars8 = NULL;
	PVK_TO_WCHARS9 pVkToWchars9 = NULL;
	PVK_TO_WCHARS10 pVkToWchars10 = NULL;
	PMODIFIERS pCharModifiers;
	PDEADKEY pDeadKey;

	HINSTANCE loadKeyboardLayout()
	{
		PKBDTABLES pKbd;
		HINSTANCE kbdLibrary;
		KbdLayerDescriptor pKbdLayerDescriptor = NULL;

		char layoutFile[MAX_PATH];
		if(getKeyboardLayoutFile(layoutFile, sizeof(layoutFile)) == -1)
		return NULL;

		char systemDirectory[MAX_PATH];
		GetSystemDirectory(systemDirectory, MAX_PATH);

		char kbdLayoutFilePath[MAX_PATH];
		_snprintf(kbdLayoutFilePath, MAX_PATH, "%s\\%s", systemDirectory, layoutFile);

		kbdLibrary = LoadLibrary(kbdLayoutFilePath);

		pKbdLayerDescriptor = (KbdLayerDescriptor)GetProcAddress(kbdLibrary, "KbdLayerDescriptor");

		if(pKbdLayerDescriptor != NULL)
			pKbd = pKbdLayerDescriptor();
		else
			return NULL;

		int i = 0;
		do
		{
			if((pKbd->pVkToWcharTable[i].cbSize - 2) / 2 == 1)
				pVkToWchars1 = (PVK_TO_WCHARS1)pKbd->pVkToWcharTable[i].pVkToWchars;
			if((pKbd->pVkToWcharTable[i].cbSize - 2) / 2 == 2)
				pVkToWchars2 = (PVK_TO_WCHARS2)pKbd->pVkToWcharTable[i].pVkToWchars;
			if((pKbd->pVkToWcharTable[i].cbSize - 2) / 2 == 3)
				pVkToWchars3 = (PVK_TO_WCHARS3)pKbd->pVkToWcharTable[i].pVkToWchars;
			if((pKbd->pVkToWcharTable[i].cbSize - 2) / 2 == 4)
				pVkToWchars4 = (PVK_TO_WCHARS4)pKbd->pVkToWcharTable[i].pVkToWchars;
			if((pKbd->pVkToWcharTable[i].cbSize - 2) / 2 == 5)
				pVkToWchars5 = (PVK_TO_WCHARS5)pKbd->pVkToWcharTable[i].pVkToWchars;
			if((pKbd->pVkToWcharTable[i].cbSize - 2) / 2 == 6)
				pVkToWchars6 = (PVK_TO_WCHARS6)pKbd->pVkToWcharTable[i].pVkToWchars;
			if((pKbd->pVkToWcharTable[i].cbSize - 2) / 2 == 7)
				pVkToWchars7 = (PVK_TO_WCHARS7)pKbd->pVkToWcharTable[i].pVkToWchars;
			if((pKbd->pVkToWcharTable[i].cbSize - 2) / 2 == 8)
				pVkToWchars8 = (PVK_TO_WCHARS8)pKbd->pVkToWcharTable[i].pVkToWchars;
			if((pKbd->pVkToWcharTable[i].cbSize - 2) / 2 == 9)
				pVkToWchars9 = (PVK_TO_WCHARS9)pKbd->pVkToWcharTable[i].pVkToWchars;
			if((pKbd->pVkToWcharTable[i].cbSize - 2) / 2 == 10)
				pVkToWchars10 = (PVK_TO_WCHARS10)pKbd->pVkToWcharTable[i].pVkToWchars;
			/*
			INIT_PVK_TO_WCHARS(i, 1)
			INIT_PVK_TO_WCHARS(i, 2)
			INIT_PVK_TO_WCHARS(i, 3)
			INIT_PVK_TO_WCHARS(i, 4)
			INIT_PVK_TO_WCHARS(i, 5)
			INIT_PVK_TO_WCHARS(i, 6)
			INIT_PVK_TO_WCHARS(i, 7)
			INIT_PVK_TO_WCHARS(i, 8)
			INIT_PVK_TO_WCHARS(i, 9)
			INIT_PVK_TO_WCHARS(i, 10)
			*/
			i++;
		}
		while(pKbd->pVkToWcharTable[i].cbSize != 0);

		pCharModifiers = pKbd->pCharModifiers;
		pDeadKey = pKbd->pDeadKey;

		return kbdLibrary;
	}

	int unloadKeyboardLayout(HINSTANCE kbdLibrary)
	{
		if(kbdLibrary != 0)
			return (FreeLibrary(kbdLibrary) != 0);
		else
			return 0;
	}

	int convertVirtualKeyToWChar(int virtualKey, PWCHAR outputChar, PWCHAR deadChar)
	{
		int i = 0;
		short state = 0;
		int shift = -1;
		int mod = 0;
		int charCount = 0;

		WCHAR baseChar;
		WCHAR diacritic;
		*outputChar = 0;

		int capsLock = (GetKeyState(VK_CAPITAL) & 0x1);

		do
		{
			state = GetAsyncKeyState(pCharModifiers->pVkToBit[i].Vk);

			if(pCharModifiers->pVkToBit[i].Vk == VK_SHIFT)
				shift = i + 1; // Get modification number for Shift key

			if(state & ~SHRT_MAX)
			{
				if(mod == 0)
					mod = i + 1;
				else
					mod = 0; // Two modifiers at the same time!
			}
			i++;
		}

		while(pCharModifiers->pVkToBit[i].Vk != 0);

		SEARCH_VK_IN_CONVERSION_TABLE(1)
		SEARCH_VK_IN_CONVERSION_TABLE(2)
		SEARCH_VK_IN_CONVERSION_TABLE(3)
		SEARCH_VK_IN_CONVERSION_TABLE(4)
		SEARCH_VK_IN_CONVERSION_TABLE(5)
		SEARCH_VK_IN_CONVERSION_TABLE(6)
		SEARCH_VK_IN_CONVERSION_TABLE(7)
		SEARCH_VK_IN_CONVERSION_TABLE(8)
		SEARCH_VK_IN_CONVERSION_TABLE(9)
		SEARCH_VK_IN_CONVERSION_TABLE(10)

		if(*deadChar != 0) // I see dead characters...
		{
			i = 0;
			do
			{
				baseChar = (WCHAR)pDeadKey[i].dwBoth;
				diacritic = (WCHAR)(pDeadKey[i].dwBoth >> 16);

				if((baseChar == *outputChar) && (diacritic == *deadChar))
				{
					*deadChar = 0;
					*outputChar = (WCHAR)pDeadKey[i].wchComposed;
				}
				i++;
			}
			while(pDeadKey[i].dwBoth != 0);
		}
		return charCount;
	}

	int getKeyboardLayoutFile(char* layoutFile, DWORD bufferSize)
	{
		HKEY hKey;
		DWORD varType = REG_SZ;

		char kbdName[KL_NAMELENGTH];
		GetKeyboardLayoutName(kbdName);

		char kbdKeyPath[51 + KL_NAMELENGTH];
		_snprintf(kbdKeyPath, 51 + KL_NAMELENGTH,"SYSTEM\\CurrentControlSet\\Control\\Keyboard Layouts\\%s", kbdName);

		if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, (LPCTSTR)kbdKeyPath, 0, KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS)
		return -1;

		if(RegQueryValueEx(hKey, "Layout File", NULL, &varType, (LPBYTE)layoutFile, &bufferSize) != ERROR_SUCCESS)
		return -1;

		RegCloseKey(hKey);

		return 1;
	}
} // Keyboard
