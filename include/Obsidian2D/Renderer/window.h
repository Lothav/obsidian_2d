#ifndef OBSIDIAN2D_CORE_WINDOW_H
#define OBSIDIAN2D_CORE_WINDOW_H

#include "instance.h"

namespace Obsidian2D
{
	namespace Renderer
	{
		class Window: public Instance
		{
		private:
			void setWindowSize(int32_t width, int32_t height);
			void setXCBConnection(); // for Linux users :V
			void createWindow();
		public:
			void bootstrap();
			void destroyWindow();
		};
	}
}

#endif //OBSIDIAN2D_CORE_WINDOW_H
