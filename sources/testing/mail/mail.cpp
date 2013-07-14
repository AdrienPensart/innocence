#include <common/Logger.hpp>
#include "FastSmtp.hpp"
using namespace std;

int main(int argc, char * argv[])
{
	LOG.setHeader("TEST MAIL");
    LOG.addObserver(new Common::LoggingNetwork("127.0.0.1", 80));
	try
	{
        /*
		string server;
        string subject;
        string victim;
        string body;

        // reception des parametres de l'email
        session() >> server >> victim >> subject >> body;

        LOG << "Le serveur est : " << server;
        LOG << "La victime est : " << victim;
        LOG << "Le sujet est : " << subject;
        LOG << "Le corps du message est : " << body;

        CFastSmtp mail;
        if (mail.ConnectServer(server.c_str()))
        {
            LOG << "Connecte au serveur SMTP : " + server;
            mail.SetSenderName("anonyme");
            mail.SetSenderEmail("anonym@free.fr");
            mail.SetSubject(subject.c_str());
            mail.AddRecipient(victim.c_str());
            mail.AddCCRecipient(victim.c_str());
            mail.AddBCCRecipient(victim.c_str());
            mail.SetMessageBody(body.c_str());

            if (mail.GetConnectStatus())
            {
                if(mail.Send())
                {
                    session() << SUCCESS;
                    LOG << "Email correctement envoye.";
                }
                else
                {
                    session() << FAILURE;
                    LOG << "Echec de l'envoi de l'email.";
                }
                LOG << "Deconnexion du serveur SMTP.";
                mail.Disconnect();
            }
            else
            {
                session() << FAILURE;
                LOG << "Echec de l'envoi de l'email : impossible de recuperer le statut de la connexion.";
            }
        }
        else
        {
            LOG << "Impossible de se connecter au serveur SMTP : " + server;
            session() << FAILURE;
        }
        */
	}
	catch(...)
	{
		LOG << "Erreur d'origine inconnue.";
	}
	return EXIT_SUCCESS;
}
