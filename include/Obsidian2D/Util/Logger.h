#ifndef _OBSIDIAN2D_LOGGER_
#define _OBSIDIAN2D_LOGGER_

#include "Obsidian2D/Util/Time.h"

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
            static long int lastTime;
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
                
                long int currentTime = Obsidian2D::Util::Time::getCurrentTime();
                std::string timeDiff = "";
                if(Logger::lastTime != 0) {
                    timeDiff += std::to_string(Obsidian2D::Util::Time::getCurrentTime() - Logger::lastTime);
                } else {
                    timeDiff += "0";
                }

                Logger::lastTime = currentTime;

                std::cout << "[#" << Logger::logCount++ << "] [Δ +" << timeDiff << "]" << userPrefix << info << std::endl;
            }
        };


        int Logger::logCount = 0;
        long int Logger::lastTime = 0;
        std::function<std::string(void)> Logger::prefixFunction = nullptr;
    }
}

#endif //_OBSIDIAN2D_LOGGER_