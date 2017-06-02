#ifndef _OBSIDIAN2D_TIME_
#define _OBSIDIAN2D_TIME_

#include <chrono>

namespace Obsidian2D
{
    namespace Util
    {
        class Time
        {
        private:

        protected:

        public:
            static unsigned long long getCurrentTime()
            {
                return std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
            }
            
        };
    }
}

#endif //_OBSIDIAN2D_TIME_