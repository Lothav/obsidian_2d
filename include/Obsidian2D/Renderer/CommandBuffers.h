//
// Created by luizorv on 9/2/17.
//

#ifndef OBSIDIAN2D_COMMANDBUFFERS_H
#define OBSIDIAN2D_COMMANDBUFFERS_H


#include <Obsidian2D/Renderer/vulkan/vulkan.h>
#include <vector>
#include <assert.h>
#include "RenderPass.h"
#include "DescriptorSet.h"
#include "SyncPrimitives.h"
#include "VertexBuffer.h"

namespace Obsidian2D
{
	namespace Renderer
	{
		class CommandBuffers
		{

		private:

			VkDevice 							_instance_device;

			VkCommandPool 						_command_pool = nullptr;
			VkCommandBuffer 					_command_buffer = nullptr;

		public:

			CommandBuffers(VkDevice device, uint32_t queueFamilyIndex)
			{
				_instance_device = device;

				VkCommandPoolCreateInfo cmd_pool_info = {};
				cmd_pool_info.sType 			= VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
				cmd_pool_info.pNext 			= nullptr;
				cmd_pool_info.queueFamilyIndex  = queueFamilyIndex;
				cmd_pool_info.flags 			= 0;

				assert(vkCreateCommandPool(device, &cmd_pool_info, nullptr, &_command_pool) == VK_SUCCESS);

				VkCommandBufferAllocateInfo cmd = {};
				cmd.sType 						= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				cmd.pNext 			 			= nullptr;
				cmd.commandPool 	 			= _command_pool;
				cmd.level 			 			= VK_COMMAND_BUFFER_LEVEL_PRIMARY;
				cmd.commandBufferCount  		= 1;

				assert(vkAllocateCommandBuffers(_instance_device, &cmd, &_command_buffer) == VK_SUCCESS);
			}

            ~CommandBuffers()
            {
                vkFreeCommandBuffers(_instance_device, _command_pool, 1, &_command_buffer);
                vkDestroyCommandPool(_instance_device, _command_pool, nullptr);
            }

			void bindCommandBuffer (
					RenderPass* 	render_pass,
					DescriptorSet* 	descriptor_set,
					VkPipeline 		vkPipeline,
					int 			buffer_index,
					uint32_t 		width,
					uint32_t 		height,
					SyncPrimitives* sync_primitives,
					VertexBuffer* 	vertex_buffer
			) {
				VkResult res;

				VkClearValue clear_values[2];
				clear_values[0].color.float32[0] = 0.2f;
				clear_values[0].color.float32[1] = 0.2f;
				clear_values[0].color.float32[2] = 0.2f;
				clear_values[0].color.float32[3] = 0.2f;
				clear_values[1].depthStencil.depth = 1.0f;
				clear_values[1].depthStencil.stencil = 0;

				VkRenderPassBeginInfo rp_begin;
				rp_begin.sType 								= VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
				rp_begin.pNext 								= nullptr;
				rp_begin.renderPass 						= render_pass->getRenderPass();
				rp_begin.renderArea.offset.x 				= 0;
				rp_begin.renderArea.offset.y 				= 0;
				rp_begin.renderArea.extent.width 			= width;
				rp_begin.renderArea.extent.height 			= height;
				rp_begin.clearValueCount 					= 2;
				rp_begin.pClearValues 						= clear_values;

				VkCommandBufferBeginInfo cmd_buf_info = {};
				cmd_buf_info.sType 							= VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				cmd_buf_info.pNext 							= nullptr;
				cmd_buf_info.flags 							= 0;
				cmd_buf_info.pInheritanceInfo 				= nullptr;

				rp_begin.framebuffer =  render_pass->getFrameBuffer()
				[buffer_index];

				res = vkBeginCommandBuffer(_command_buffer, &cmd_buf_info);
				assert(res == VK_SUCCESS);

				vkCmdBeginRenderPass(_command_buffer, &rp_begin, VK_SUBPASS_CONTENTS_INLINE);
				vkCmdBindPipeline(_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipeline);
				vkCmdBindDescriptorSets(_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
										descriptor_set->getPipelineLayout(), 0, 1,
										descriptor_set->getDescriptorSet(), 0, nullptr);

				const VkDeviceSize offsets[1] = {0};

				//std::vector<VkBuffer> buffers = VertexBuffer::getBuffersFromVector(vertex_buffer);
				vkCmdBindVertexBuffers(_command_buffer, 0, 1, &vertex_buffer->buf, offsets);

				Util* util  = new Util(width, height);
				util->init_viewports(_command_buffer);
				util->init_scissors(_command_buffer);

				vkCmdDraw(_command_buffer, static_cast<uint32_t>(vertexData.size()), 1, 0, 0);
				vkCmdEndRenderPass(_command_buffer);
				res = vkEndCommandBuffer(_command_buffer);

				delete util;
				assert(res == VK_SUCCESS);
			}

			VkCommandBuffer* getCommandBuffer()
			{
				return &_command_buffer;
			}

			VkCommandPool getCommandPool()
			{
				return _command_pool;
			}
		};
	}
}

#endif //OBSIDIAN2D_COMMANDBUFFERS_H
