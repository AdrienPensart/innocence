#pragma once

#include <string>

namespace Inhibition
{
    class ProgramStartupKey
    {
        public:

            ProgramStartupKey(const std::string& program_path);
            ~ProgramStartupKey();

            void setPath(const std::string& program_path);
            void install();
            void uninstall();
            bool installed();

        private:

            std::string programPath;
    };

} // Inhibition
