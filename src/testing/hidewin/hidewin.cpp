#include <malicious/InternetExplorer.hpp>
#include <log/Log.hpp>

#include <common/Innocence.hpp>
#include <audit/Audit.hpp>

int main()
{
    try
	{
        LOG.setIdentity(Common::identity);
		LOG.addObserver(new Log::LogToConsole);
	    LOG.addObserver(new Log::LogToCollector);
		LOG.addObserver(new Audit::LogToAuditor);
        LOG.trace();
		
		Malicious::InternetExplorer ie;

		LOG << "Internet Explorer PID : " + Common::toString(ie.getPid());

		system("pause");

		ie.kill();
	}
	catch(...)
	{
		LOG << "Erreur d'origine inconnue.";
	}
}