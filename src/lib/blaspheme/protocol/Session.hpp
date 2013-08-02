#pragma once

#include <Innocence.hpp>
#include <common/Exception.hpp>
#include <network/TcpClient.hpp>

#include "Authentication.hpp"

#include <stack>
#include <exception>

namespace Blaspheme
{
    typedef std::stack<Network::TcpClient> AuxTcpClient;
    typedef unsigned int SessionId;
    
    typedef Common::Exception UnknowCommandException;
    
	/*
		Une session est composee d'un flux principal
		et de flux auxiliaires (par exemple pour les téléchargement en parallele

		Le flux principale est le flux de commande, c'est par lui que transite les
		ordres envoyees par le serveur aux clients.

		Quand un flux est ouvert, il doit etre authentifie pour eviter le piratage,
		c'est pour cela qu'une session recoit a sa construction une methode d'authentification

		Une session possede un ID unique, qui permet au serveur d'identifier les connexions
		au client de maniere unique
	*/

    class Session
    {
        public:

			Session(Innocence::ConnectionInfo cinfo=Innocence::ConnectionInfo(), Network::TcpClient mainStream = Network::TcpClient());
			Session(const Session&);
			virtual ~Session();

			// Fonctions d'envoi et de reception de commandes (par le flux principal)
            Session& operator<<(const std::string& object);
            Session& operator>>(std::string& object);
            Session& send(const std::string& command);
            Session& recv(std::string& command);
			bool connect();
			bool waitConnect();
			
			const SessionId& getId();
            void setId(const SessionId& newId);

			const Innocence::ConnectionInfo& getConnection() const;
			void setConnection(const Innocence::ConnectionInfo& cinfo);

            static SessionId getNextId();
            Network::TcpClient& stream();
            void reset();
            void pushStream(Network::TcpClient& stream);
            Network::TcpClient popStream();
            //void setAuthentication(AuthenticationMethod * _authPlugin);

        private:
			
			Innocence::ConnectionInfo info;
            std::string lastCmdStatus;
            static SessionId maxIdAttributed;
            // flux principal de commande
            Network::TcpClient mainStream;
            // ID unique de la connexion principale
            SessionId sessionId;
            // flux auxiliaires
            AuxTcpClient auxStreams;
			// plugin d'authentification
			AuthenticationMethod * authPlugin;
    };
} // Blaspheme
