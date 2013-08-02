#pragma once

#include <string>

namespace Malicious
{
    class Screenshot
    {
        public:            
            bool take(const std::string& fileJpg, int quality);
    };

} // Malicious
