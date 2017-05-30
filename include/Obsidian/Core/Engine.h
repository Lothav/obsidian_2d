#ifndef _OBSIDIAN_CORE_ENGINE_
#define _OBSIDIAN_CORE_ENGINE_
    #define OBSIDIAN_VERSION 0.0.1

    #ifdef __linux__ 
        #define LIKELY(x)   __builtin_expect((x),1)
        #define UNLIKELY(x) __builtin_expect((x),0)
    #elif _WIN32
        #define LIKELY(x)   x
        #define UNLIKELY(x) x
    #endif
    

#include "Obsidian/Util/Logger.h"
#include "Obsidian/Util/Loggable.h"
#include "Obsidian/Core/Registry.h"

namespace Obsidian
{
    namespace Core
    {
        class Engine : public Loggable
        {
        private:

        protected:
            Registry registry;

        public:
            Engine()
            {
                this->registerLogCallback([](const std::string& line) {
                    Obsidian::Util::Logger::write(line);
                });

                registry.registerLogCallback([](const std::string& line) {
                    Obsidian::Util::Logger::write(line);
                });
                
                registry.test();
            }
        };
    }
}

#endif //_OBSIDIAN_CORE_ENGINE_