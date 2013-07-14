//!
//!     Kaleidoscalp, all rights reserved.
//!

#ifndef _BINARY_RESSOURCE_HPP
#define _BINARY_RESSOURCE_HPP

#include <cstdio>
#include <string>
#include <common/Logger.hpp>

namespace Malicious
{

    class BinaryRessource
    {
	    public:

		    BinaryRessource(const unsigned char * ressourceName, size_t size, const std::string& _ressourceFile, const bool& _deleteIt=false)
			    :ressourceFile(_ressourceFile), deleteIt(_deleteIt)
		    {
			    deleteRessource();
			    HANDLE hFile = CreateFile(ressourceFile.c_str(),GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
			    DWORD wmWritten = 0; 
			    WriteFile(hFile, ressourceName,(DWORD)size, &wmWritten, NULL); 
			    CloseHandle(hFile); 
			    if(wmWritten != size)
			    {
				    FATAL_ERROR("Ecriture du fichier " + ressourceFile + " impossible ou incomplete.");
			    }
		    }

		    ~BinaryRessource()
		    {
			    if(deleteIt)
			    {
				    if(!deleteRessource())
				    {
					    FATAL_ERROR("Impossible d'effacer " + ressourceFile);
				    }
			    }
		    }

	    private:
    		
		    bool deleteRessource()
		    {
			    DWORD attrib = GetFileAttributes(ressourceFile.c_str());
			    if(attrib!=INVALID_FILE_ATTRIBUTES)
			    {
				    return DeleteFile(ressourceFile.c_str()) != 0;
			    }
			    return true;
		    }

		    std::string ressourceFile;
		    bool deleteIt;

    };

} // Malicious

#endif // _BINARY_RESSOURCE_HPP
