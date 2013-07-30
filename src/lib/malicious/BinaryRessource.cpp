#include "BinaryRessource.hpp"

namespace Malicious
{
	BinaryRessource::BinaryRessource(const unsigned char * ressourceName, std::size_t size, const std::string& _ressourceFile, const bool& _deleteIt)
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

    BinaryRessource::~BinaryRessource()
    {
	    if(deleteIt)
	    {
		    if(!deleteRessource())
		    {
			    FATAL_ERROR("Impossible d'effacer " + ressourceFile);
		    }
	    }
    }

	bool BinaryRessource::deleteRessource()
    {
	    DWORD attrib = GetFileAttributes(ressourceFile.c_str());
	    if(attrib!=INVALID_FILE_ATTRIBUTES)
	    {
		    return DeleteFile(ressourceFile.c_str()) != 0;
	    }
	    return true;
    }
} // Malicious
