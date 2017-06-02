#ifndef OBSIDIAN2D_CORE_MAIN_H
#define OBSIDIAN2D_CORE_MAIN_H

#include "Obsidian2D/Util/VulkanInfo.h"

#define APP_NAME "Obsidian2D"

namespace Obsidian2D
{
	namespace Renderer
	{
		namespace Vulkan
		{
			class Instance
			{
				struct VulkanInfo info = {};
				VkResult setGlobalLayerProperties(struct VulkanInfo &info);
				VkResult setGlobalExtensionProperties(layer_properties &layer_props);
				VkApplicationInfo setApplicationInfo();
				VkResult setInstanceInfo(VkApplicationInfo app_info);
			public:
				void bootstrap();
			};
		}
	}
}

#endif //OBSIDIAN2D_CORE_MAIN_H
