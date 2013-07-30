#ifndef _SCREENSHOT_
#define _SCREENSHOT_

#include <string>

namespace Malicious
{
    class Screenshot
    {
        public:            
            bool take(const std::string& fileJpg, int quality);
    };

} // Malicious

#endif // _SCREENSHOT_
