#ifndef _OBSIDIAN_CORE_REGISTRY_
#define _OBSIDIAN_CORE_REGISTRY_
    #define OBSIDIAN_VERSION 0.0.1

    #ifdef __linux__ 
        #define LIKELY(x)   __builtin_expect((x),1)
        #define UNLIKELY(x) __builtin_expect((x),0)
    #elif _WIN32
        #define LIKELY(x)   x
        #define UNLIKELY(x) x
    #endif
    

#include "Obsidian/Util/Loggable.h"

namespace Obsidian
{
    namespace Core
    {
        class Registry : public Loggable
        {
        private:

        protected:

        public:
            Registry()
            {
                
            }

            void test()
            {
                //this->log("a");
            }
        };
    }
}

#endif //_OBSIDIAN_CORE_REGISTRY_