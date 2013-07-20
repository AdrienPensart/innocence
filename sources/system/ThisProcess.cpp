#include "ThisProcess.hpp"
#include <windows.h>
#include <common/Log.hpp>
#include <system/File.hpp>

namespace System
{
	ThisProcess::ThisProcess()
		:argc(0), argvw(0), argv(0)
	{
		// récuperation des arguments au programme
		argvw = CommandLineToArgvW(GetCommandLineW(), &argc);
		if(argvw == NULL)
		{
			FATAL_ERROR("Impossible de recuperer les arguments du programme.");
		}

		// Conversion en chaine multi-octets
		argv = new char * [argc];
		for(int index = 0; index < argc; index++)
		{
			size_t stringSize = wcslen(argvw[index]);
			argv[index] = new char [stringSize+1];
			wcstombs(argv[index], argvw[index], stringSize+1);
		}

		programPath = argv[0];
		programName = programPath;
		programDir = programPath;
		System::GetFileName(programName);
		System::GetFileDir(programDir);
	}

	ThisProcess::~ThisProcess()
	{
		LocalFree(argvw);
		for(int index = 0; index < argc; index++)
		{
			delete argv[index];
		}
		delete argv;
	}

	const std::string& ThisProcess::getProgramPath()
	{
		return programPath;
	}

	const std::string& ThisProcess::getProgramName()
	{
		return programName;
	}

	const std::string& ThisProcess::getProgramDir()
	{
		return programDir;
	}

	int& ThisProcess::getArgCount()
	{
		return argc;
	}

	const std::string ThisProcess::getArg(unsigned int index)
	{
		return argv[index];
	}

	char ** ThisProcess::getArgs()
	{
		return argv;
	}

} /* System */
