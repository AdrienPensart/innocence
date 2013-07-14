#ifndef _SCREENSHOT_
#define _SCREENSHOT_

#include <string>

namespace Inhibition
{

    class Screenshot
    {
        public:            
            bool take(const std::string& fileJpg, int quality);
    };

} /* Inhibition */

#endif // _SCREENSHOT_
