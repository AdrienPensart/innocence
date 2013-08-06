#include <malicious/InternetExplorer.hpp>
#include <common/Log.hpp>
using namespace Malicious;

int main()
{
    try
	{
        LOG.setHeader("TEST HIDEWIN");
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