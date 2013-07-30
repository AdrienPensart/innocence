#include "InhibitionCoreImpl.hpp"
#include "InhibitionCore.hpp"

namespace Inhibition
{

    InhibitionCore::InhibitionCore()
    {
        core = new InhibitionCoreImpl;
    }

    InhibitionCore::~InhibitionCore()
    {
        delete core;
    }

    bool InhibitionCore::connect()
    {
        return core->connect();
    }

    bool InhibitionCore::process_command()
    {
        return core->process_command();
    }
        
    bool InhibitionCore::exit()
    {
        return core->exit();
    }
    
    void InhibitionCore::disconnect()
    {
        core->disconnect();
    }
    
    bool InhibitionCore::acquire_stream()
    {
        return core->acquire_stream();
    }
    
    bool InhibitionCore::uninstall()
    {
        return core->uninstall();
    }
            
    void InhibitionCore::upgrade()
    {
        core->upgrade();
    }
            
    const std::string& InhibitionCore::getInstallPath()
    {
        return core->getInstallPath();
    }
    
    void InhibitionCore::set_connection_infos(const Blaspheme::ConnectionInfo& info)
    {
        core->set_connection_infos(info);
    }

}
