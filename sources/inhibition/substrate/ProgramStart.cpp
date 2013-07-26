#include <system/Registry.hpp>
#include <common/Log.hpp>
#include <blaspheme/Blaspheme.hpp>
#include <string>
using namespace System::Registry;
using namespace std;

#include "ProgramStart.hpp"

namespace Inhibition
{
    ProgramStartupKey::ProgramStartupKey(const string& path)
        :programPath(path)
    {
        if(!installed())
        {
            LOG << "Installing automatic starting registry key";
            install();
        }
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
        Key startKey(localMachine, RUN_KEY_PATH);
        return startKey.isValueExists(RUN_KEY_NAME);
    }

    void ProgramStartupKey::install()
    {
        Key startKey(localMachine, RUN_KEY_PATH);
        startKey.createValue(RUN_KEY_NAME, programPath);
    }

    void ProgramStartupKey::uninstall()
    {
        Key startKey(localMachine, RUN_KEY_PATH);
        if(startKey.isValueExists(RUN_KEY_NAME))
        {
            startKey.removeValue(RUN_KEY_NAME);
        }
    }
}
