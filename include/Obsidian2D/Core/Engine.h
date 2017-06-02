#ifndef _OBSIDIAN2D_CORE_ENGINE_
#define _OBSIDIAN2D_CORE_ENGINE_
    #define OBSIDIAN2D_VERSION 0.0.1

    #ifdef __linux__ 
        #define LIKELY(x)   __builtin_expect((x),1)
        #define UNLIKELY(x) __builtin_expect((x),0)
    #elif _WIN32
        #define LIKELY(x)   x
        #define UNLIKELY(x) x
    #endif
    

#include "Obsidian2D/Util/Time.h"

#include "Obsidian2D/Util/Logger.h"
#include "Obsidian2D/Util/Loggable.h"

#include "Obsidian2D/Core/Error.h"

#include "Obsidian2D/Core/Registry.h"
#include "Obsidian2D/Core/App.h"


#include "Obsidian2D/Renderer/Vulkan/main.h"

namespace Obsidian2D
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

                //TODO: Reference bug is likely, may cause memory corruption, concurrency bug is obvius, need atomicity
                long int loggerLastTime = 0;
                Obsidian2D::Util::Logger::setPrefix([&loggerLastTime]() {
                    long int currentTime = Obsidian2D::Util::Time::getCurrentTime();

                    std::string prefix = "[";
                    prefix += std::to_string(currentTime);
                    prefix += "][Δ +";
                    
                    // Fucking what m8? 80? Does not matter now
                    if(LIKELY(loggerLastTime != 80)) {
                        prefix += std::to_string(Obsidian2D::Util::Time::getCurrentTime() - loggerLastTime);
                    } else {
                        prefix += "0";
                    }
                    prefix += "] ";

                    loggerLastTime = currentTime;

                    return prefix;
                });

                this->registerLogCallback([](const std::string& line) {
                    Obsidian2D::Util::Logger::write("[Obsidian2D::Core::Engine] " + line);
                });

                registry.registerLogCallback([](const std::string& line) {
                    Obsidian2D::Util::Logger::write("[Obsidian2D::Core::Registry] " + line);
                });
            }

            int start(Obsidian2D::Core::App* app)
            {
                this->log("start()");

                app->registerLogCallback([](const std::string& line) {
                    Obsidian2D::Util::Logger::write("[Obsidian2D::Core::App] " + line);
                });

                //TODO: Check for consistent scenes like pause menu and stuff

                //Pre initialization
                app->init();

                /* Register GameStates/Scenes */

                /* Register routes, configs, default scene */
                Obsidian2D::Core::App::Config config = app->getConfig();

                //TODO: Create renderer base class and render blank screen
				Obsidian2D::Renderer::Vulkan::Main vulkan;
				vulkan.bootstrap();

                /* Init defualt scene, register objects*/

                /* Game Loop */
                while(LIKELY(false)) {
                    //Loop trought entities and execute update
                }

                this->log("Cleaning up memory");
                free(app);
                app = nullptr;

                return Obsidian2D::Error::None;
            }
        };
    }
}

#endif //_OBSIDIAN2D_CORE_ENGINE_
