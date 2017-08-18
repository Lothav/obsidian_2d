#ifndef _OBSIDIAN2D_UTIL_LOGGABLE_
#define _OBSIDIAN2D_UTIL_LOGGABLE_

#include <functional>
#include <iostream>

namespace Obsidian2D
{
	namespace Util
	{
		class Loggable
		{
		private:
			std::function<void(const std::string&)> logCallback = nullptr;

		protected:
			Loggable() {};
			virtual ~Loggable() {}

			void log(const std::string& info)
			{
				if(this->logCallback != nullptr) {
					this->logCallback(info);
				} else {
					std::cout << info << std::endl;
				}
			}
		public:
			void registerLogCallback(std::function<void(const std::string&)> lambda)
			{
				this->logCallback = lambda;
			}
		};
	}
}
#endif // _OBSIDIAN2D_UTIL_LOGGABLE_
