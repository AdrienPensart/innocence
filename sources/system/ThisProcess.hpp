#ifndef _THIS_PROCESS_HPP_
#define _THIS_PROCESS_HPP_

#include <string>

namespace System
{

	// Cette classe prend en charge les arguments du processus, le nom et 
	// le chemin de l'exécutable, et les infos sur le processus (PID)
	class ThisProcess
	{
		public:

			ThisProcess();
			~ThisProcess();

			const std::string& getProgramPath();
			const std::string& getProgramName();
			const std::string& getProgramDir();
			int& getArgCount();
			const std::string getArg(unsigned int index);
			char ** getArgs();

		private:

			wchar_t ** argvw;
			char ** argv;
			int argc;
			std::string programPath;
			std::string programName;
			std::string programDir;
	};

} /* System */

#endif // _THIS_PROCESS_HPP_
