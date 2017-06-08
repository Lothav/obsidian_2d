#ifndef OBSIDIAN2D_CORE_WINDOW_H
#define OBSIDIAN2D_CORE_WINDOW_H

#include "instance.h"
#include "Obsidian2D/Util/Loggable.h"

namespace Obsidian2D
{
	namespace Renderer
	{
		class Window: public Instance, public Loggable
		{
		private:
			int32_t width;
			int32_t height;

			void setWindowSize();
			void setXCBConnection(); // for Linux users :V
			void createWindow();
			void initSwapChainExtension();
		public:
			Window(int32_t width, int32_t height){
				this->width = width;
				this->height = height;
			}
			void bootstrap();
			void destroyWindow();
		};
	}
}

#endif //OBSIDIAN2D_CORE_WINDOW_H
