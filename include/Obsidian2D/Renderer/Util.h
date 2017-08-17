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
			VkCommandBuffer 						command_buffer;
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

			void init_viewports()
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
				vkCmdSetViewport(command_buffer, 0, 1, &viewport);
#endif
			}

			void init_scissors()
			{
#ifdef __ANDROID__
				// Disable dynamic viewport on Android. Some drive has an issue with the dynamic scissors feature.
#else
				VkRect2D scissor;
				scissor.extent.width =  (uint32_t)width;
				scissor.extent.height = (uint32_t)height;
				scissor.offset.x = 0;
				scissor.offset.y = 0;
				vkCmdSetScissor(command_buffer, 0, 1, &scissor);
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

			void set_image_layout(VkImage image, VkImageAspectFlags aspectMask, VkImageLayout old_image_layout,
								  VkImageLayout new_image_layout, VkPipelineStageFlags src_stages, VkPipelineStageFlags dest_stages)
			{
				/* DEPENDS on info.cmd and info.queue initialized */

				assert(command_buffer != VK_NULL_HANDLE);
				assert(graphics_queue != VK_NULL_HANDLE);

				VkImageMemoryBarrier image_memory_barrier = {};
				image_memory_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				image_memory_barrier.pNext = NULL;
				image_memory_barrier.srcAccessMask = 0;
				image_memory_barrier.dstAccessMask = 0;
				image_memory_barrier.oldLayout = old_image_layout;
				image_memory_barrier.newLayout = new_image_layout;
				image_memory_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				image_memory_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				image_memory_barrier.image = image;
				image_memory_barrier.subresourceRange.aspectMask = aspectMask;
				image_memory_barrier.subresourceRange.baseMipLevel = 0;
				image_memory_barrier.subresourceRange.levelCount = 1;
				image_memory_barrier.subresourceRange.baseArrayLayer = 0;
				image_memory_barrier.subresourceRange.layerCount = 1;

				switch (old_image_layout) {
					case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
						image_memory_barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
						break;

					case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
						image_memory_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
						break;

					case VK_IMAGE_LAYOUT_PREINITIALIZED:
						image_memory_barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
						break;

					default:
						break;
				}

				switch (new_image_layout) {
					case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
						image_memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
						break;

					case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
						image_memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
						break;

					case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
						image_memory_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
						break;

					case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
						image_memory_barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
						break;

					case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
						image_memory_barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
						break;

					default:
						break;
				}

				vkCmdPipelineBarrier(command_buffer, src_stages, dest_stages, 0, 0, NULL, 0, NULL, 1, &image_memory_barrier);
			}

			void write_ppm(const char *basename)
			{
				std::string filename;
				VkResult res;

				VkImageCreateInfo image_create_info = {};
				image_create_info.sType 									 		= VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
				image_create_info.pNext 									 		= NULL;
				image_create_info.imageType 								 		= VK_IMAGE_TYPE_2D;
				image_create_info.format 									 		= format;
				image_create_info.extent.width 										= (uint32_t) width;
				image_create_info.extent.height 									= (uint32_t) height;
				image_create_info.extent.depth 										= 1;
				image_create_info.mipLevels 								 		= 1;
				image_create_info.arrayLayers 								 		= 1;
				image_create_info.samples 									 		= VK_SAMPLE_COUNT_1_BIT;
				image_create_info.tiling 									 		= VK_IMAGE_TILING_LINEAR;
				image_create_info.initialLayout 							 		= VK_IMAGE_LAYOUT_UNDEFINED;
				image_create_info.usage 									 		= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
				image_create_info.queueFamilyIndexCount 					 		= 0;
				image_create_info.pQueueFamilyIndices 						 		= NULL;
				image_create_info.sharingMode 								 		= VK_SHARING_MODE_EXCLUSIVE;
				image_create_info.flags 									 		= 0;

				VkMemoryAllocateInfo mem_alloc = {};
				mem_alloc.sType 													= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				mem_alloc.pNext 													= NULL;
				mem_alloc.allocationSize 											= 0;
				mem_alloc.memoryTypeIndex 											= 0;

				VkImage mappableImage;
				VkDeviceMemory mappableMemory;

				/* Create a mappable image */
				res = vkCreateImage(device, &image_create_info, NULL, &mappableImage);
				assert(res == VK_SUCCESS);

				VkMemoryRequirements mem_reqs;
				vkGetImageMemoryRequirements(device, mappableImage, &mem_reqs);

				mem_alloc.allocationSize = mem_reqs.size;

				/* Find the memory type that is host mappable */
				bool pass = memory_type_from_properties(
						memory_properties,
						mem_reqs.memoryTypeBits,
						VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
						&mem_alloc.memoryTypeIndex
				);
				assert(pass && "No mappable, coherent memory");

				/* allocate memory */
				res = vkAllocateMemory(device, &mem_alloc, NULL, &(mappableMemory));
				assert(res == VK_SUCCESS);

				/* bind memory */
				res = vkBindImageMemory(device, mappableImage, mappableMemory, 0);
				assert(res == VK_SUCCESS);

				VkCommandBufferBeginInfo cmd_buf_info = {};
				cmd_buf_info.sType 										= VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				cmd_buf_info.pNext 										= NULL;
				cmd_buf_info.flags 										= 0;
				cmd_buf_info.pInheritanceInfo 							= NULL;

				res = vkBeginCommandBuffer(command_buffer, &cmd_buf_info);
				assert(res == VK_SUCCESS);

				this->set_image_layout(mappableImage, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_UNDEFINED,
									   VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT);

				this->set_image_layout(buffers[current_buffer].image, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
									   VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT);

				VkImageCopy copy_region;
				copy_region.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				copy_region.srcSubresource.mipLevel = 0;
				copy_region.srcSubresource.baseArrayLayer = 0;
				copy_region.srcSubresource.layerCount = 1;
				copy_region.srcOffset.x = 0;
				copy_region.srcOffset.y = 0;
				copy_region.srcOffset.z = 0;
				copy_region.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				copy_region.dstSubresource.mipLevel = 0;
				copy_region.dstSubresource.baseArrayLayer = 0;
				copy_region.dstSubresource.layerCount = 1;
				copy_region.dstOffset.x = 0;
				copy_region.dstOffset.y = 0;
				copy_region.dstOffset.z = 0;
				copy_region.extent.width =  (uint32_t)width;
				copy_region.extent.height = (uint32_t)height;
				copy_region.extent.depth = 1;

				/* Put the copy command into the command buffer */
				vkCmdCopyImage(command_buffer, buffers[current_buffer].image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, mappableImage,
							   VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy_region);

				this->set_image_layout(mappableImage, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_GENERAL,
									   VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_HOST_BIT);

				res = vkEndCommandBuffer(command_buffer);
				assert(res == VK_SUCCESS);
				const VkCommandBuffer cmd_bufs[] = {command_buffer};
				VkFenceCreateInfo fenceInfo;
				VkFence cmdFence;
				fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
				fenceInfo.pNext = NULL;
				fenceInfo.flags = 0;
				vkCreateFence(device, &fenceInfo, NULL, &cmdFence);

				VkSubmitInfo submit_info[1] = {};
				submit_info[0].pNext = NULL;
				submit_info[0].sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
				submit_info[0].waitSemaphoreCount = 0;
				submit_info[0].pWaitSemaphores = NULL;
				submit_info[0].pWaitDstStageMask = NULL;
				submit_info[0].commandBufferCount = 1;
				submit_info[0].pCommandBuffers = cmd_bufs;
				submit_info[0].signalSemaphoreCount = 0;
				submit_info[0].pSignalSemaphores = NULL;

				/* Queue the command buffer for execution */
				res = vkQueueSubmit(graphics_queue, 1, submit_info, cmdFence);
				assert(res == VK_SUCCESS);

				/* Make sure command buffer is finished before mapping */
				do {
					res = vkWaitForFences(device, 1, &cmdFence, VK_TRUE, 100000000);
				} while (res == VK_TIMEOUT);
				assert(res == VK_SUCCESS);

				vkDestroyFence(device, cmdFence, NULL);

				filename.append(basename);
				filename.append(".ppm");

				VkImageSubresource subres = {};
				subres.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				subres.mipLevel = 0;
				subres.arrayLayer = 0;
				VkSubresourceLayout sr_layout;
				vkGetImageSubresourceLayout(device, mappableImage, &subres, &sr_layout);

				char *ptr;
				res = vkMapMemory(device, mappableMemory, 0, mem_reqs.size, 0, (void **)&ptr);
				assert(res == VK_SUCCESS);

				ptr += sr_layout.offset;
				vkUnmapMemory(device, mappableMemory);
				vkDestroyImage(device, mappableImage, NULL);
				vkFreeMemory(device, mappableMemory, NULL);
			}


		};
	}
}
