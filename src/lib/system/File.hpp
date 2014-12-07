#pragma once

#include <string>
#include <fstream>

namespace System
{
	#ifdef WIN32
	class FileListing
	{
		public:
			std::string getLogicalVolumes();
			std::string getDirectoryList(const std::string& directory);
	};
	#endif

	std::string GetFileDir(const std::string& pathname);
	std::string GetFileName(const std::string& pathname);
	std::string GetFileBase(const std::string& pathname);
	std::string ReadLine(std::fstream& handle);
	std::string ReadAll(std::fstream& handle);
	bool isReadable( const std::string & file);

	std::streamsize Size(std::ifstream& handle);
	std::streamsize Size(std::fstream& handle);
	std::streamsize Size(std::wfstream& handle);
	std::streamsize Size(const std::string& file);

} // System
