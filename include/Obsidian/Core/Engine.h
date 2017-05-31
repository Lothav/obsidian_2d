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
    

#include "Obsidian/Util/Time.h"

#include "Obsidian/Util/Logger.h"
#include "Obsidian/Util/Loggable.h"


#include "Obsidian/Core/Registry.h"
#include "Obsidian/Core/App.h"

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

                //TODO: Make logger prefix which class/module in the hierarchy calls it
                Obsidian::Util::Logger::setPrefix([]() {
                    std::string prefix = "[";
                    
                    prefix += std::to_string(Obsidian::Util::Time::getCurrentTime());
                    prefix += "] ";

                    return prefix;
                });

                this->registerLogCallback([](const std::string& line) {
                    Obsidian::Util::Logger::write("[Obsidian::Core::Engine] " + line);
                });

                registry.registerLogCallback([](const std::string& line) {
                    Obsidian::Util::Logger::write("[Obsidian::Core::Registry] " + line);
                });
            }

            int start(Obsidian::Core::App* app)
            {
                this->log("start()");

                app->registerLogCallback([](const std::string& line) {
                    Obsidian::Util::Logger::write("[Obsidian::Core::App] " + line);
                });

                //TODO: Check for consistent scenes like pause menu and stuff

                //Pre initialization
                app->init();

                /* Register GameStates/Scenes */

                /* Register routes, configs, default scene */
                Obsidian::Core::App::Config config = app->getConfig();

                //TODO: Create renderer base class and render blank screen
                if(config.renderer == "default") {
                    //load vulkan
                }
                /* Init Renderer, audio, Resource manager, AI, Input based on App Config*/

                /* Init defualt scene, register objects*/

                /* Game Loop */
                while(LIKELY(false)) {
                    //Loop trought entities and execute update
                }

                free(app);
                app = nullptr;
                return 0;
            }
        };
    }
}

#endif //_OBSIDIAN_CORE_ENGINE_
