#ifndef OBSIDIAN2D_CORE_MAIN_H
#define OBSIDIAN2D_CORE_MAIN_H

#include "Obsidian2D/Util/VulkanInfo.h"

namespace Obsidian2D
{
	namespace Renderer
	{
		namespace Vulkan
		{
			class Main
			{
				struct VulkanInfo info = {};
				VkResult setGlobalLayerProperties(struct VulkanInfo &info);
				VkResult setGlobalExtensionProperties(layer_properties &layer_props);
			public:
				void bootstrap();
			};
		}
	}
}

#endif //OBSIDIAN2D_CORE_MAIN_H
