//
// Created by luizorv on 8/25/17.
//

#include <vulkan/vulkan.h>
#include <assert.h>
#include <cstring>
#include "./stb/stb_image.h"
#include "Memory.h"
#include "Buffer.h"

#ifndef OBSIDIAN2D_TEXTURES_H
#define OBSIDIAN2D_TEXTURES_H

namespace Obsidian2D
{
	namespace Renderer
	{
		class Textures
		{

		private:

			VkDevice 				device;

			static void createImage(VkDevice device, uint32_t width, uint32_t height, VkFormat format,
								  	VkImageTiling tiling, VkImageUsageFlags usage,
									VkMemoryPropertyFlags properties, VkImage& image,
									VkDeviceMemory& imageMemory)
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

				res = vkCreateImage(device, &imageInfo, nullptr, &image);
				assert(res == VK_SUCCESS);

				VkMemoryRequirements memRequirements;
				vkGetImageMemoryRequirements(device, image, &memRequirements);

				VkMemoryAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				allocInfo.allocationSize = memRequirements.size;

				//allocInfo.memoryTypeIndex = Memory::findMemoryType(memRequirements.memoryTypeBits, properties);

				res = vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory);
				assert(res == VK_SUCCESS);

				vkBindImageMemory(device, image, imageMemory, 0);
			}

		public:

			static void createTextureImage(VkPhysicalDevice physicalDevice, VkDevice device, const char* path)
			{
				VkResult 				res;
				VkImage 				textureImage;
				VkDeviceMemory 			textureImageMemory;

				int texWidth, texHeight, texChannels;
				stbi_uc* pixels = stbi_load(path, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
				VkDeviceSize imageSize = static_cast<VkDeviceSize>(texWidth * texHeight * 4);

				assert(pixels);

				VkBuffer stagingBuffer;
				VkDeviceMemory stagingBufferMemory;

				Buffer * staging_buffer = new Buffer();

				staging_buffer->createBuffer (
						physicalDevice, device, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
						VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
				);

				void* data;
				vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
				memcpy(data, pixels, static_cast<size_t>(imageSize));
				vkUnmapMemory(device, stagingBufferMemory);

				stbi_image_free(pixels);

				createImage(device, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight),
							VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL,
							VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
							VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory);
			}

			static VkImageView createImageView(VkDevice device, VkImage image, VkFormat format) {

				VkResult res;

				VkImageViewCreateInfo viewInfo = {};
				viewInfo.sType 								= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				viewInfo.image 								= image;
				viewInfo.viewType 							= VK_IMAGE_VIEW_TYPE_2D;
				viewInfo.format 							= format;
				viewInfo.subresourceRange.aspectMask 		= VK_IMAGE_ASPECT_COLOR_BIT;
				viewInfo.subresourceRange.baseMipLevel 		= 0;
				viewInfo.subresourceRange.levelCount 		= 1;
				viewInfo.subresourceRange.baseArrayLayer 	= 0;
				viewInfo.subresourceRange.layerCount 		= 1;

				VkImageView imageView;

				res = vkCreateImageView(device, &viewInfo, nullptr, &imageView);

				assert(res == VK_SUCCESS);

				return imageView;
			}

		};
	}
}

#endif //OBSIDIAN2D_TEXTURES_H
