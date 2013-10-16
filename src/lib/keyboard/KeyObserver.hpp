#pragma once

#include <common/WindowsWrapper.hpp>
#include <map>
#include <string>
#include <fstream>

namespace Keyboard
{
	typedef struct
    {
        DWORD vkCode;
        DWORD scanCode;
        DWORD flags;
        DWORD time;
        unsigned long * dwExtraInfo;
    } KBDLLHOOKSTRUCT, *PKBDLLHOOKSTRUCT;

	#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
	#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

	class KeyInformation
	{
		public:
			KBDLLHOOKSTRUCT hooked;
	};

	class KeyObserver
	{
		public:
			KeyObserver()
			{
			}

			virtual ~KeyObserver()
			{
			}

			virtual void update(const KeyInformation& ki)=0;

		private:
			bool blockKey;
	};
} // Keyboard
