#ifndef _OBSIDIAN_TIME_
#define _OBSIDIAN_TIME_

#include <chrono>

namespace Obsidian
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

#endif //_OBSIDIAN_TIME_