#include <common/Log.hpp>
#include <system/Registry.hpp>
using namespace System;
using namespace System::Registry;
using namespace std;

// le but du test est de mettre le test lui meme au démarrage du PC
int main(int argc, char * argv[])
{
    static const char * RUN_KEY = "Software\\Microsoft\\Windows\\CurrentVersion\\Run\\";
	LOG.setHeader("TEST REGISTRE");
    LOG.addObserver(new Common::LogToNetwork("127.0.0.1", 80));
	try
	{
        LOG << "Chemin du programme : " + to_string(argv[0]);
        Key startKey(localMachine, RUN_KEY);
        startKey.createValue("test", argv[0]);

        SHOW_BOX("REGISTRE", "effacement de la valeur...");

        startKey.removeValue("test");
	}
	catch(RegistryBaseError&)
	{
		LOG << "Mauvais choix de base de registre.";
	}
	catch(KeyDoesntExist&)
	{
		LOG << "Erreur la cle n'existe pas.";
	}
	catch(KeyErrorCreate&)
	{
		LOG << "Erreur lors de la creation de la cle.";
	}
	catch(RegistryError&)
	{
		LOG << "Erreur de registre.";
	}
	catch(...)
	{
		LOG << "Erreur d'origine inconnue.";
	}
	return EXIT_SUCCESS;
}
