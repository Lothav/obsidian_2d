#ifndef _OBSIDIAN_CORE_APP_
#define _OBSIDIAN_CORE_APP_

#include "Obsidian/Util/Loggable.h"

namespace Obsidian
{
    namespace Core
    {
        class App : public Loggable
        {
        public:
            //TODO: Check if i could do this in an App namespace or something better, like another file
            struct Config
            {
                std::string renderer = "default";
            };

        private:

        protected:

        public:
            App()
            {

            }

            void init()
            {
                this->log("init()");
            }

            virtual Obsidian::Core::App::Config getConfig()
            {
                return Config();
            }
        };
    }
}

#endif //_OBSIDIAN_CORE_APP_