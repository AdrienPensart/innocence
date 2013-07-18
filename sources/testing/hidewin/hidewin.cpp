#include <malicious/InternetExplorer.hpp>
#include <common/Logger.hpp>
using namespace Malicious;

int main()
{
    try
	{
        LOG.setHeader("TEST HIDEWIN");
	    LOG.addObserver(new Common::LoggingNetwork("127.0.0.1", 80));
		LOG.addObserver(new Common::LoggingMessage());
        LOG.trace();
		
		Malicious::InternetExplorer ie;

		LOG << "Internet Explorer PID : "+to_string(ie.getPid());

		system("pause");

		ie.kill();
	}
	catch(...)
	{
		LOG << "Erreur d'origine inconnue.";
	}
}