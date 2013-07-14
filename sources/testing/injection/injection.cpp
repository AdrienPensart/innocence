#include <common/Logger.hpp>
#include <malicious/Injector.hpp>
#include <system/Process.hpp>

int main(int argc, char * argv[])
{
	try
	{
        LOG.setHeader("TEST INJECTION");
        LOG.addObserver(new Common::LoggingNetwork("127.0.0.1", 80));

        System::Process injected_process("iexplore.exe", "about:blank");
        if(!injected_process.isRunning())
        {
            FATAL_ERROR("Impossible de demarrer le processus a injecter.");
        }

        if(Malicious::inject(injected_process.getPid(), "dll.dll"))
        {
		    LOG << "DLL Injected";
        }
        else
        {
            LOG << "Injection failed.";
        }
    }
    catch(std::exception& e)
    {
        LOG << "Exception standard : " + to_string(e.what());
    }
	catch(...)
	{
		LOG << "Erreur d'origine inconnue.";
	}
	return EXIT_SUCCESS;
}
