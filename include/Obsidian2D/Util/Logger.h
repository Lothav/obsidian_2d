#ifndef _OBSIDIAN2D_LOGGER_
#define _OBSIDIAN2D_LOGGER_

#include <iostream>
#include <string>
#include <functional>

namespace Obsidian2D
{
    namespace Util
    {
        class Logger
        {
        private:

        protected:
            static int logCount;
            static std::function<std::string(void)> prefixFunction;

        public:

            static void setPrefix(std::function<std::string(void)> prefixUserFunction)
            {
                Logger::prefixFunction = prefixUserFunction;
            }

            static void write(const std::string& info)
            {
                std::string userPrefix = "";
                if(Logger::prefixFunction != nullptr) {
                    userPrefix = Logger::prefixFunction();
                }
                
                std::cout << "[#" << Logger::logCount++ << "] " << userPrefix << info << std::endl;
            }
        };

        int Logger::logCount = 0;
        std::function<std::string(void)> Logger::prefixFunction = nullptr;
    }
}

#endif //_OBSIDIAN2D_LOGGER_