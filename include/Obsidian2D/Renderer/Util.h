//
// Created by luizorv on 6/10/17.
//

#ifndef OBSIDIAN2D_CORE_UTIL_H
#define OBSIDIAN2D_CORE_UTIL_H

#include "Obsidian2D/Util/Loggable.h"

#endif //OBSIDIAN2D_CORE_UTIL_H
namespace Obsidian2D
{
	namespace Renderer
	{
		class Util : public Loggable
		{
		protected:

			struct VulkanInfo info = {};
			void init_viewports() {
#ifdef __ANDROID__
				// Disable dynamic viewport on Android. Some drive has an issue with the dynamic viewport
// feature.
#else
				this->info.viewport.height = (float)this->info.height;
				this->info.viewport.width = (float)this->info.width;
				this->info.viewport.minDepth = (float)0.0f;
				this->info.viewport.maxDepth = (float)1.0f;
				this->info.viewport.x = 0;
				this->info.viewport.y = 0;
				vkCmdSetViewport(info.cmd, 0, 1, &this->info.viewport);
#endif
			}

			void init_scissors() {
#ifdef __ANDROID__
				// Disable dynamic viewport on Android. Some drive has an issue with the dynamic scissors
// feature.
#else
				this->info.scissor.extent.width =  (uint32_t)this->info.width;
				this->info.scissor.extent.height = (uint32_t)this->info.height;
				this->info.scissor.offset.x = 0;
				this->info.scissor.offset.y = 0;
				vkCmdSetScissor(this->info.cmd, 0, 1, &this->info.scissor);
#endif
			}

			void wait_seconds(int seconds) {
#ifdef WIN32
				Sleep(seconds * 1000);
#elif defined(__ANDROID__)
				sleep(seconds);
#else
				sleep(seconds);
#endif
			}


		};
	}
}