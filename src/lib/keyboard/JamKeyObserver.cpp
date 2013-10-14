#include "JamKeyObserver.hpp"
#include "Keylogger.hpp"

#include <log/Log.hpp>

namespace Keyboard
{
	JamKeyObserver::JamKeyObserver(const std::vector<int>& keyCombArg) : 
		keyComb(keyCombArg)
	{
	}

	void JamKeyObserver::update(const KeyInformation& ki)
	{
		KBDLLHOOKSTRUCT hooked = ki.hooked;
        DWORD vk_key = hooked.vkCode;

		for(KeyCombination::iterator iter1 = keyComb.begin();
			iter1 != keyComb.end();
			iter1++)
		{
			if(vk_key == *iter1)
			{
				LOG << "One key of combination found";
				for(KeyCombination::iterator iter2 = keyComb.begin();
					iter2 != keyComb.end();
					iter2++)
				{
					if(KEY_UP(*iter2))
					{
						return;
					}
				}
				LOG << "Combination blocked";
				Keylogger::instance().blockNextKey();
				break;
			}
		}
		/*
        char ascii_key = (char)MapVirtualKey(hooked.vkCode, 2);
        bool caps_locked = (((unsigned short)GetKeyState(0x14)) & 0xffff) != 0;
        bool maj = KEY_UP(VK_SHIFT) ^ caps_locked;

		
		DWORD dwMsg = 1;
		dwMsg += hooked.scanCode << 16;
		dwMsg += hooked.flags << 24;

		char lpszName[256] = {0};
		int i = GetKeyNameText(dwMsg,lpszName,0xFF) + 1;
		std::string buffer = lpszName;
		buffer = "[" + buffer + "]";

		HWND hActive = GetForegroundWindow();
		DWORD dwProcessId;
		DWORD dwThreadID = GetWindowThreadProcessId(hActive, &dwProcessId);
		HKL hKL = GetKeyboardLayout(dwThreadID);
		BYTE *kState = (BYTE*)malloc(256);
		GetKeyboardState(kState);
		
		UINT nScanCode = MapVirtualKeyEx(nChar, 2, hKL);
		WORD wScanKey = LOWORD(nScanCode);

		UINT nConv = ToAsciiEx(nChar, nScanCode, kState, szKey, 0, hKL);
		*/
	}
} // Keyboard
