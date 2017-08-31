//
// Created by luizorv on 8/27/17.
//

#ifndef OBSIDIAN2D_BUFFER_H
#define OBSIDIAN2D_BUFFER_H

#include <Obsidian2D/Renderer/vulkan/vulkan.h>
#include <assert.h>
#include "Memory.h"

namespace Obsidian2D
{
	namespace Renderer
	{
		class Buffer
		{

		private:
			VkDevice _instance_device;

		public:

			VkBuffer 									buf;
			VkDeviceMemory 								mem;
			VkDescriptorBufferInfo 						buffer_info;

			~Buffer()
			{
				vkDestroyBuffer(_instance_device, buf, nullptr);
				vkFreeMemory(_instance_device, mem, nullptr);
			}

			Buffer(VkPhysicalDevice physicalDevice, VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage, VkFlags properties)
			{
				this->_instance_device = device;

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

				res = vkCreateBuffer(device, &bufferInfo, nullptr, &this->buf);
				assert(res == VK_SUCCESS);

				VkMemoryRequirements memRequirements;
				vkGetBufferMemoryRequirements(device, this->buf, &memRequirements);

				VkPhysicalDeviceMemoryProperties memProperties;
				vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

				VkMemoryAllocateInfo allocInfo = {};
				allocInfo.sType 						= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				allocInfo.allocationSize 				= memRequirements.size;
				allocInfo.pNext 						= nullptr;

				pass = Memory::findMemoryType (
						memProperties,
						memRequirements.memoryTypeBits,
						properties,
						&allocInfo.memoryTypeIndex);

				assert(pass);

				res = vkAllocateMemory(device, &allocInfo, nullptr, &this->mem);
				assert(res == VK_SUCCESS);

				vkBindBufferMemory(device, this->buf, this->mem, 0);

				this->buffer_info.range 				= size;
				this->buffer_info.offset 				= 0;
				this->buffer_info.buffer 				= this->buf;
			}
		};
	}
}
#endif //OBSIDIAN2D_BUFFER_H
