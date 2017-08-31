//
// Created by tracksale on 8/31/17.
//

#ifndef OBSIDIAN2D_BUFFERIMAGE_H
#define OBSIDIAN2D_BUFFERIMAGE_H

#include <Obsidian2D/Renderer/vulkan/vulkan.h>
#include <assert.h>
#include "Memory.h"

namespace Obsidian2D
{
    namespace Renderer
    {
        class BufferImage
        {

        private:
            VkDevice _instance_device;
            VkPhysicalDeviceMemoryProperties _memory_properties;

        public:

            VkImage 		image;
            VkDeviceMemory 	mem;
            VkImageView 	view;

            ~BufferImage()
            {
                vkDestroyImage(_instance_device, image, nullptr);
                vkDestroyImageView(_instance_device, view, nullptr);
                vkFreeMemory(_instance_device, mem, nullptr);
            }

            BufferImage (
                    VkDevice device, VkPhysicalDeviceMemoryProperties memoryProperties,
                    uint32_t width, uint32_t height, VkFormat format,
                    VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties)
            {
                this->_instance_device = device;
                this->_memory_properties = memoryProperties;
                createImage(width, height, format, tiling, usage, properties);
                createImageView();
                allocateMemory();
            }

        private:

            void createImageView (VkImageAspectFlags aspectMask = VK_IMAGE_ASPECT_COLOR_BIT)
            {
                VkResult res;

                VkImageViewCreateInfo viewInfo = {};
                viewInfo.sType 								= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
                viewInfo.image 								= image;
                viewInfo.viewType 							= VK_IMAGE_VIEW_TYPE_2D;
                viewInfo.format 							= VK_FORMAT_UNDEFINED;
                viewInfo.subresourceRange.aspectMask 		= aspectMask;
                viewInfo.subresourceRange.baseMipLevel 		= 0;
                viewInfo.subresourceRange.levelCount 		= 1;
                viewInfo.subresourceRange.baseArrayLayer 	= 0;
                viewInfo.subresourceRange.layerCount 		= 1;

                res = vkCreateImageView(_instance_device, &viewInfo, nullptr, &this->view);
                assert(res == VK_SUCCESS);
            }

            void allocateMemory ()
            {
                VkResult res;
                VkMemoryRequirements mem_reqs;

                vkGetImageMemoryRequirements(_instance_device, image, &mem_reqs);

                VkMemoryAllocateInfo mem_alloc = {};
                mem_alloc.sType				= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
                mem_alloc.pNext				= NULL;
                mem_alloc.allocationSize 	= 0;
                mem_alloc.memoryTypeIndex 	= 0;

                mem_alloc.allocationSize = mem_reqs.size;
                /* Use the memory properties to determine the type of memory required */
                bool pass = Memory::findMemoryType(
                        _memory_properties,
                        mem_reqs.memoryTypeBits,
                        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                        &mem_alloc.memoryTypeIndex
                );
                assert(pass);

                res = vkAllocateMemory(_instance_device, &mem_alloc, NULL, &mem);
                assert(res == VK_SUCCESS);
                res = vkBindImageMemory(_instance_device, image, mem, 0);
                assert(res == VK_SUCCESS);
            }

            void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties)
            {
                VkResult res;

                VkImageCreateInfo imageInfo = {};
                imageInfo.sType 					 = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
                imageInfo.imageType 				 = VK_IMAGE_TYPE_2D;
                imageInfo.extent.width 				 = width;
                imageInfo.extent.height 			 = height;
                imageInfo.extent.depth 				 = 1;
                imageInfo.mipLevels 				 = 1;
                imageInfo.arrayLayers 				 = 1;
                imageInfo.format 					 = format;
                imageInfo.tiling 					 = tiling;
                imageInfo.initialLayout 			 = VK_IMAGE_LAYOUT_UNDEFINED;
                imageInfo.usage 					 = usage;
                imageInfo.samples 					 = VK_SAMPLE_COUNT_1_BIT;
                imageInfo.sharingMode 				 = VK_SHARING_MODE_EXCLUSIVE;

                res = vkCreateImage(_instance_device, &imageInfo, nullptr, &image);
                assert(res == VK_SUCCESS);

                VkMemoryRequirements memRequirements;
                vkGetImageMemoryRequirements(_instance_device, image, &memRequirements);

                VkMemoryAllocateInfo allocInfo = {};
                allocInfo.sType 					= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
                allocInfo.allocationSize 			= memRequirements.size;

                Memory::findMemoryType(
                        _memory_properties,
                        memRequirements.memoryTypeBits,
                        properties,
                        &allocInfo.memoryTypeIndex
                );

                res = vkAllocateMemory(_instance_device, &allocInfo, nullptr, &mem);
                assert(res == VK_SUCCESS);
            }
        };
    }
}
#endif //OBSIDIAN2D_BUFFERIMAGE_H
