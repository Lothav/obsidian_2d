//
// Created by luizorv on 6/10/17.
//

#ifndef OBSIDIAN2D_CORE_UTIL_H
#define OBSIDIAN2D_CORE_UTIL_H

#if defined(NDEBUG) && defined(__GNUC__)
#define U_ASSERT_ONLY __attribute__((unused))
#else
#define U_ASSERT_ONLY
#endif

#include <vulkan/vulkan.h>
#include <xcb/xcb.h>
#include <vector>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "Obsidian2D/Util/Loggable.h"

typedef struct _swap_chain_buffers {
	VkImage image;
	VkImageView view;
} swap_chain_buffer;

#endif //OBSIDIAN2D_CORE_UTIL_H
namespace Obsidian2D
{
	namespace Renderer
	{
		class Util : public Obsidian2D::Util::Loggable
		{
		protected:
			VkDevice 								device;
			int32_t									width;
			int32_t									height;
			std::vector<VkCommandBuffer>			command_buffer;
			VkFormat 								format;
			VkQueue 								graphics_queue, present_queue;
			VkPhysicalDeviceMemoryProperties 		memory_properties;
			std::vector<swap_chain_buffer> 			buffers;
			uint32_t 								current_buffer = 0;

			bool memory_type_from_properties(VkPhysicalDeviceMemoryProperties memory_properties, uint32_t typeBits, VkFlags requirements_mask, uint32_t *typeIndex)
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

			void init_viewports(VkCommandBuffer cmd_buffer)
			{
#ifdef __ANDROID__
				// Disable dynamic viewport on Android. Some drive has an issue with the dynamic viewport
// feature.
#else
				VkViewport viewport;
				viewport.height = (float)height;
				viewport.width = (float)width;
				viewport.minDepth = (float)0.0f;
				viewport.maxDepth = (float)1.0f;
				viewport.x = 0;
				viewport.y = 0;
				vkCmdSetViewport(cmd_buffer, 0, 1, &viewport);
#endif
			}

			void init_scissors(VkCommandBuffer cmd_buffer)
			{
#ifdef __ANDROID__
				// Disable dynamic viewport on Android. Some drive has an issue with the dynamic scissors feature.
#else
				VkRect2D scissor;
				scissor.extent.width =  (uint32_t)width;
				scissor.extent.height = (uint32_t)height;
				scissor.offset.x = 0;
				scissor.offset.y = 0;
				vkCmdSetScissor(cmd_buffer, 0, 1, &scissor);
#endif
			}

			void wait_seconds(int seconds)
			{
#ifdef WIN32
				Sleep(seconds * 1000);
#elif defined(__ANDROID__)
				sleep(seconds);
#else
				//sleep(seconds);
#endif
			}
		};
	}
}
