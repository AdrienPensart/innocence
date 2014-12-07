#include "File.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <common/WindowsWrapper.hpp>
using namespace std;

namespace System
{
	std::string GetFileDir(const std::string& pathname)
	{
		size_t last = pathname.find_last_of('\\');
		return pathname.substr(0, last);
	}

	std::string GetFileName(const std::string& pathname)
	{
		size_t last = pathname.find_last_of('\\');
		return pathname.substr(last+1);
	}

	std::string GetFileBase(const std::string& pathname)
	{
		size_t last = pathname.find_last_of('\\');
		size_t extension = pathname.find_last_of('.');
		return pathname.substr(last+1, extension-last-1);
	}

	std::string ReadLine(std::fstream& handle)
	{
		std::string object;
		std::getline(handle, object);
		return object;
	}

	std::string ReadAll(std::fstream& handle)
	{
		std::stringstream buffer;
		buffer << handle.rdbuf();
		return buffer.str();
	}

	bool isReadable(const std::string& file)
	{
		std::ifstream fichier( file.c_str() );
		return !fichier.fail();
	}

	#ifdef WIN32
	std::string FileListing::getLogicalVolumes()
	{
		DWORD lecteurs = GetLogicalDrives();
		static DWORD modif_bit[] = { 1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200,
							  0x400, 0x800, 0x1000, 0x2000, 0x4000, 0x8000, 0x10000,
							  0x20000, 0x40000, 0x80000, 0x100000, 0x200000, 0x400000,
							  0x800000, 0x1000000, 0x2000000 };

		static const char * liste_lecteur[] = { "A:", "B:", "C:", "D:", "E:", "F:", "G:", "H:", "I:",
								   "J:", "K:", "L:", "M:", "N:", "O:", "P:", "Q:",
								   "R:", "S:", "T:", "U:", "V:", "W:", "Y:", "Z:"};
		std::string volumes;
		for(int i = 0; i < 25; i++)
		{
			if(lecteurs & modif_bit[i])
			{
				volumes += liste_lecteur[i];
				volumes += "\\\n";
			}
		}
		return volumes;
	}

	std::string FileListing::getDirectoryList(const std::string& directory_to_list)
	{
		std::string str_directory = directory_to_list + "*";
		WIN32_FIND_DATA file;
		std::string content;

		HANDLE hSearch = FindFirstFile(str_directory.c_str(), &file);
		if (hSearch != INVALID_HANDLE_VALUE)
		{
			do
			{
				// on saute les fichiers spéciaux . et ..
				if(!(strncmp(file.cFileName, ".", MAX_PATH) == 0 || strncmp(file.cFileName, "..", MAX_PATH) == 0))
				{
					content += file.cFileName;

					// si c'est un dossier, on rajoute un \ à la fin
					if(file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						content += "\\";
					}
					content += "\n";
				}
			} while (FindNextFile(hSearch, &file));

			FindClose(hSearch);
		}
		return content;
	}
	#endif

	std::streamsize Size(std::ifstream& handle)
	{
		// sauvegarder la position courante
		std::streamoff pos = handle.tellg();
		// se placer en fin de fichier
		handle.seekg( 0 , std::ios_base::end );
		// récupérer la nouvelle position = la taille du fichier
		std::streamoff size = handle.tellg();
		// restaurer la position initiale du fichier
		handle.seekg( pos,  std::ios_base::beg ) ;
		return size ;
	}

	std::streamsize Size(std::fstream& handle)
	{
		// sauvegarder la position courante
		std::streamoff pos = handle.tellg();
		// se placer en fin de fichier
		handle.seekg( 0 , std::ios_base::end );
		// récupérer la nouvelle position = la taille du fichier
		std::streamoff size = handle.tellg();
		// restaurer la position initiale du fichier
		handle.seekg( pos,  std::ios_base::beg ) ;
		return size ;
	}

	std::streamsize Size(std::wfstream& handle)
	{
		// sauvegarder la position courante
		std::streamoff pos = handle.tellg();
		// se placer en fin de fichier
		handle.seekg( 0 , std::ios_base::end );
		// récupérer la nouvelle position = la taille du fichier
		std::streamoff size = handle.tellg();
		// restaurer la position initiale du fichier
		handle.seekg( pos,  std::ios_base::beg ) ;
		return size ;
	}

	std::streamsize Size(const std::string& file)
	{
		std::ifstream fichier(file.c_str());
		if(!fichier.fail())
		{
			// sauvegarder la position courante
			std::streamoff pos = fichier.tellg();
			// se placer en fin de fichier
			fichier.seekg( 0 , std::ios_base::end );
			// récupérer la nouvelle position = la taille du fichier
			std::streamoff size = fichier.tellg() ;
			// restaurer la position initiale du fichier
			fichier.seekg( pos,  std::ios_base::beg ) ;
			return size;
		}
		return 0;
	}
} // System
