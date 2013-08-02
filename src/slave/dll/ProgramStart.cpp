#include <system/Registry.hpp>
#include <common/Log.hpp>
#include <Innocence.hpp>
#include <string>
using namespace System::Registry;
using namespace std;

#include "ProgramStart.hpp"

namespace Inhibition
{
    ProgramStartupKey::ProgramStartupKey(const string& path)
        :programPath(path)
    {
#ifndef INNOCENCE_DEBUG
        if(!installed())
        {
            LOG << "Installing automatic starting registry key";
            install();
        }
#else
		LOG << "Automatic Run key disabled in DEBUG";
#endif
    }

    ProgramStartupKey::~ProgramStartupKey()
    {
    }

    void ProgramStartupKey::setPath(const string& path)
    {
        programPath = path;
    }

    bool ProgramStartupKey::installed()
    {
        Key startKey(localMachine, Innocence::RUN_KEY_PATH);
        return startKey.isValueExists(Innocence::RUN_KEY_NAME);
    }

    void ProgramStartupKey::install()
    {
        Key startKey(localMachine, Innocence::RUN_KEY_PATH);
        startKey.createValue(Innocence::RUN_KEY_NAME, programPath);
    }

    void ProgramStartupKey::uninstall()
    {
        Key startKey(localMachine, Innocence::RUN_KEY_PATH);
        if(startKey.isValueExists(Innocence::RUN_KEY_NAME))
        {
            startKey.removeValue(Innocence::RUN_KEY_NAME);
        }
    }
}
