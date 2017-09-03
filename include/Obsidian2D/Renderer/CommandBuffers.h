//
// Created by luizorv on 9/2/17.
//

#ifndef OBSIDIAN2D_COMMANDBUFFERS_H
#define OBSIDIAN2D_COMMANDBUFFERS_H


#include <Obsidian2D/Renderer/vulkan/vulkan.h>
#include <vector>
#include <assert.h>

namespace Obsidian2D
{
	namespace Renderer
	{
		class CommandBuffers
		{

		private:

			VkDevice 							_instance_device;

			VkCommandPool 						_command_pool = nullptr;
			std::vector<VkCommandBuffer> 		_command_buffers = {};
			int 								_index;

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
			}

			void createCommandBuffer()
			{
				VkCommandBuffer cmd_buffer;

				VkCommandBufferAllocateInfo cmd = {};
				cmd.sType 				= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				cmd.pNext 			 	= nullptr;
				cmd.commandPool 	 	= _command_pool;
				cmd.level 			 	= VK_COMMAND_BUFFER_LEVEL_PRIMARY;
				cmd.commandBufferCount  = 1;

				assert(vkAllocateCommandBuffers(_instance_device, &cmd, &cmd_buffer) == VK_SUCCESS);

				_command_buffers.push_back(cmd_buffer);
			}

			void bindCommandBuffer()
			{
				int i;
				VkResult res;
/*
				VkCommandBufferBeginInfo cmd_buf_info = {};
				cmd_buf_info.sType 							= VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				cmd_buf_info.pNext 							= NULL;
				cmd_buf_info.flags 							= 0;
				cmd_buf_info.pInheritanceInfo 				= NULL;

				res = vkBeginCommandBuffer(_command_buffers[_index], &cmd_buf_info);
				assert(res == VK_SUCCESS);
				vkCmdBeginRenderPass(_command_buffers[_index], &rp_begin, VK_SUBPASS_CONTENTS_INLINE);

				//@TODO create sync primitives
				//vkCreateFence(_instance_device, &fenceInfo, NULL, &drawFence[i]);

				vkCmdBindPipeline(_command_buffers[_index], VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipeline);
				vkCmdBindDescriptorSets(_command_buffers[_index], VK_PIPELINE_BIND_POINT_GRAPHICS,
										pipeline_layout, 0, 1, &desc_set, 0, NULL);

				const VkDeviceSize offsets[1] = {0};

				std::vector<VkBuffer> buffers = VertexBuffer::getBuffersFromVector(vertex_buffer);
				vkCmdBindVertexBuffers(_command_buffers[_index], 0, static_cast<uint32_t >(buffers.size()), buffers.data(), offsets);

				this->init_viewports(_command_buffers[_index]);
				this->init_scissors(_command_buffers[_index]);

				vkCmdDraw(_command_buffers[_index], static_cast<uint32_t>(vertexData.size()), 1, 0, 0);
				vkCmdEndRenderPass(_command_buffers[_index]);
				res = vkEndCommandBuffer(_command_buffers[_index]);
				assert(res == VK_SUCCESS);*/
			}


		};
	}
}

#endif //OBSIDIAN2D_COMMANDBUFFERS_H
