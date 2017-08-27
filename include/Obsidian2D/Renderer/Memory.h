//
// Created by luizorv on 8/25/17.
//

#ifndef OBSIDIAN2D_MEMORY_H
#define OBSIDIAN2D_MEMORY_H

#include <vulkan/vulkan.h>

namespace Obsidian2D
{
	namespace Renderer
	{
		class Memory
		{
		public:
			static bool memory_type_from_properties(VkPhysicalDeviceMemoryProperties memory_properties, uint32_t typeBits, VkFlags requirements_mask, uint32_t *typeIndex)
			{
				// Search memtypes to find first index with those properties
				for (uint32_t i = 0; i < memory_properties.memoryTypeCount; i++) {
					if ((typeBits & 1) == 1) {
						// Type is available, does it match user properties?
						if ((memory_properties.memoryTypes[i].propertyFlags & requirements_mask) == requirements_mask) {
							*typeIndex = i;
							return true;
						}
					}
					typeBits >>= 1;
				}
				// No memory types matched, return failure
				return false;
			}

			/*static uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkDevice device) {
				VkPhysicalDeviceMemoryProperties memProperties;
				vkGetPhysicalDeviceMemoryProperties(device, &memProperties);

				for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
					if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
						return i;
					}
				}

			}*/

		};
	}
}

#endif //OBSIDIAN2D_MEMORY_H
