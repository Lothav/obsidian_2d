#ifndef OBSIDIAN2D_CORE_MAIN_H
#define OBSIDIAN2D_CORE_MAIN_H

#include "Obsidian2D/Util/VulkanInfo.h"

#define APP_NAME "Obsidian2D"

namespace Obsidian2D
{
	namespace Renderer
	{
		class Instance
		{
		private:
			struct VulkanInfo info = {};
			VkDevice device;

			VkResult setGlobalLayerProperties(struct VulkanInfo &info);
			VkResult setGlobalExtensionProperties(layer_properties &layer_props);
			VkApplicationInfo setApplicationInfo();
			VkResult setInstanceInfo(VkApplicationInfo app_info);
			void enumerateDevice(uint32_t gpu_count);
			void createDevice();
		public:
			void bootstrap();
			void destroyInstance();
			void destroyDevice();
		};
	}
}

#endif //OBSIDIAN2D_CORE_MAIN_H
