//
// Created by luizorv on 8/27/17.
//

#ifndef OBSIDIAN2D_BUFFERS_H
#define OBSIDIAN2D_BUFFERS_H

#include <Obsidian2D/Renderer/vulkan/vulkan.h>
#include <assert.h>
#include "Memory.h"

namespace Obsidian2D
{
	namespace Renderer
	{
		class Buffers
		{
		public:
			static void createBuffer (
					VkPhysicalDevice physicalDevice,
					VkDevice device,
					VkDeviceSize size,
					VkBufferUsageFlags usage,
					VkFlags properties,
					VkBuffer * buffer,
					VkDeviceMemory * bufferMemory)
			{

				VkResult res;
				bool pass;

				VkBufferCreateInfo bufferInfo = {};

				bufferInfo.sType 						= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
				bufferInfo.size 						= size;
				bufferInfo.usage 						= usage;
				bufferInfo.queueFamilyIndexCount		= 0;
				bufferInfo.pQueueFamilyIndices 			= nullptr;
				bufferInfo.sharingMode 					= VK_SHARING_MODE_EXCLUSIVE;
				bufferInfo.flags 						= 0;
				bufferInfo.pNext 						= nullptr;

				res = vkCreateBuffer(device, &bufferInfo, nullptr, buffer);
				assert(res == VK_SUCCESS);

				VkMemoryRequirements memRequirements;
				vkGetBufferMemoryRequirements(device, *buffer, &memRequirements);

				VkPhysicalDeviceMemoryProperties memProperties;
				vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

				VkMemoryAllocateInfo allocInfo = {};
				allocInfo.sType 						= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				allocInfo.allocationSize 				= memRequirements.size;
				allocInfo.pNext 						= nullptr;

				pass = Memory::findMemoryType(
						memProperties,
						memRequirements.memoryTypeBits,
						properties,
						&allocInfo.memoryTypeIndex);

				assert(pass);

				res = vkAllocateMemory(device, &allocInfo, nullptr, bufferMemory);
				assert( res == VK_SUCCESS );

				vkBindBufferMemory(device, *buffer, *bufferMemory, 0);
			}
		};
	}
}
#endif //OBSIDIAN2D_BUFFERS_H
