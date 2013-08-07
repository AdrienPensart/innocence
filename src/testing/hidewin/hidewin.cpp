#include <malicious/InternetExplorer.hpp>
#include <common/Log.hpp>
using namespace Malicious;

#include <Innocence.hpp>

int main()
{
    try
	{
        LOG.setIdentity(Innocence::identity);
	    LOG.addObserver(new Common::LogToCollector);
		LOG.addObserver(new Common::LogToConsole);
        LOG.trace();
		
		Malicious::InternetExplorer ie;

		LOG << "Internet Explorer PID : "+toString(ie.getPid());

		system("pause");

		ie.kill();
	}
	catch(...)
	{
		LOG << "Erreur d'origine inconnue.";
	}
}