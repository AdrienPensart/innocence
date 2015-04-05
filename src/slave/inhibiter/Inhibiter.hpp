#pragma once

#include <string>

namespace Inhibiter {
	class InhibiterCore {
		public:

			InhibiterCore(const std::string& executable);
			void install();
			void inject();
			void uninstall();
			void finishUninstall();
			// est ce que l'exécutable actuel est celui installé dans le repertoire definitif
			// ou vient-il juste d'etre téléchargé ?
			bool installed();
			// innocence a-t-il déja été installé ?
			bool isEverInstalled();
			std::string getExecutablePath() {
				return current_executable_path;
			}
			std::string getInjectedProcess() {
				return processToInject;
			}

		private:

			std::string processToInject;
			std::string current_executable_path;
			std::string install_directory;
			std::string executable_path;
			std::string dll_path;
	};

} // Inhibiter
