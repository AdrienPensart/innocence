#include "BinaryRessource.hpp"

#include <common/Exception.hpp>
#include <Windows.h>

namespace Malicious
{
	BinaryRessource::BinaryRessource(const unsigned char * ressourceName, std::size_t size, const std::string& _ressourceFile, const bool& _deleteIt)
		:ressourceFile(_ressourceFile), deleteIt(_deleteIt)
	{
	    deleteRessource();
	    HANDLE hFile = CreateFile(ressourceFile.c_str(),GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	    DWORD wmWritten = 0; 
	    WriteFile(hFile, ressourceName,(DWORD)size, &wmWritten, NULL); 
	    
	    if(wmWritten != size)
	    {
			Common::Exception writingError("Error during writing of file " + ressourceFile + " : " + toString(GetLastError()));
			CloseHandle(hFile);
		    throw writingError;
	    }
    }

    BinaryRessource::~BinaryRessource()
    {
	    if(deleteIt)
	    {
		    deleteRessource();
	    }
    }

	void BinaryRessource::deleteRessource()
    {
	    DWORD attrib = GetFileAttributes(ressourceFile.c_str());
	    if(attrib != INVALID_FILE_ATTRIBUTES)
	    {
		    if(!DeleteFile(ressourceFile.c_str()))
			{
				throw Common::Exception("Unable to delete file : " + ressourceFile + " : " + toString(GetLastError()));
			}
	    }
    }
} // Malicious
