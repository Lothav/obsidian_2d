//
// Created by luizorv on 7/22/17.
//

#ifndef OBSIDIAN2D_CORE_WINDOW2_H
#define OBSIDIAN2D_CORE_WINDOW2_H

#include "UniformBuffer.h"
#include "Memory.h"
#include "Textures.h"
#include "BufferImage.h"
#include "VertexBuffer.h"
#include "RenderPass.h"
#include "DescriptorSet.h"
#include "GraphicPipeline.h"

#define APP_NAME "Obsidian2D"

namespace Obsidian2D
{
	namespace Renderer
	{
		class Window : public Layers {

		public:

			VkInstance 								instance;
			VkSurfaceKHR 							surface;

			~Window()
			{
				uint32_t i;

				delete render_pass;

                if (surface != VK_NULL_HANDLE)
                {
                    vkDestroySurfaceKHR(instance, surface, nullptr);
                }

				//vkDestroyPipeline(device, vkPipeline, NULL);
				//vkDestroyPipelineCache(device, pPipelineCache, NULL);

				delete descriptor_set;
				delete vertex_buffer;

				// Destroy shaders
				//vkDestroyShaderModule(device, shaderStages[0].module, NULL);
				//vkDestroyShaderModule(device, shaderStages[1].module, NULL);

				vkFreeCommandBuffers(device, _command_pool, (u_int32_t)command_buffer.size(), command_buffer.data());

				vkDestroySemaphore(device, imageAcquiredSemaphore, nullptr);
				vkDestroySemaphore(device, renderSemaphore, nullptr);

				for(i = 0; i < drawFence.size(); i++){
					vkDestroyFence(device, drawFence[i], nullptr);
				}

                vkFreeMemory(device, Textures::textureImageMemory, nullptr);

                vkDestroyCommandPool(device, _command_pool, NULL);
                vkDestroyDevice(this->device, NULL);
				vkDestroyInstance(instance, NULL);
			}

            void draw()
            {
                VkResult res;
				VkSwapchainKHR swap_c = render_pass->getSwapChain()->getSwapChainKHR();

                descriptor_set->getUniformBuffer()->updateCamera(device);

                res = vkAcquireNextImageKHR(device, swap_c, UINT64_MAX, imageAcquiredSemaphore, VK_NULL_HANDLE, &current_buffer);
                assert(res == VK_SUCCESS);

                VkPipelineStageFlags pipe_stage_flags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

                VkSubmitInfo submit_info;
                submit_info.pNext                     = NULL;
                submit_info.sType                     = VK_STRUCTURE_TYPE_SUBMIT_INFO;
                submit_info.waitSemaphoreCount        = 1;
                submit_info.pWaitSemaphores           = &imageAcquiredSemaphore;
                submit_info.pWaitDstStageMask         = &pipe_stage_flags;
                submit_info.commandBufferCount        = 1;
                submit_info.pCommandBuffers           = &command_buffer[current_buffer];
                submit_info.signalSemaphoreCount      = 1;
                submit_info.pSignalSemaphores         = &renderSemaphore;

                res = vkQueueSubmit(render_pass->getSwapChain()->getGraphicQueue(), 1, &submit_info, drawFence[current_buffer]);
                assert(res == VK_SUCCESS);

                do {
                    res = vkWaitForFences(device, 1, &drawFence[current_buffer], VK_TRUE, VK_SAMPLE_COUNT_1_BIT);
                } while (res == VK_TIMEOUT);
                assert(res == VK_SUCCESS);
                vkResetFences(device, 1, &drawFence[current_buffer]);

                VkPresentInfoKHR present;

                present.sType 				           = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
                present.pNext 				           = NULL;
                present.swapchainCount 		           = 1;
                present.pSwapchains 		           = &swap_c;
                present.pImageIndices 		           = &current_buffer;
                present.pWaitSemaphores 	           = NULL;
                present.waitSemaphoreCount 	           = 0;
                present.pResults                       = NULL;

                if (renderSemaphore != VK_NULL_HANDLE)
                {
                    present.pWaitSemaphores = &renderSemaphore;
                    present.waitSemaphoreCount = 1;
                }

                res = vkQueuePresentKHR(render_pass->getSwapChain()->getPresentQueue(), &present);
                assert(res == VK_SUCCESS);

                vkDeviceWaitIdle(device);
            }

		private:
			VkPhysicalDeviceProperties 				gpu_props;
			VkSemaphore         					imageAcquiredSemaphore;
			VkSemaphore         					renderSemaphore;
			std::vector<VkFence>					drawFence;
			VkCommandPool							_command_pool;
			VkDevice 								device;
			std::vector<VkCommandBuffer>			command_buffer;
			VkPhysicalDeviceMemoryProperties 		memory_properties;
			uint32_t 								current_buffer = 0;
			std::vector<VkPhysicalDevice> 			gpu_vector;
			u_int32_t							 	queue_family_count;
			std::vector<VkQueueFamilyProperties> 	queue_family_props;

			RenderPass* 							render_pass;
		protected:
            VertexBuffer *             				vertex_buffer;
			DescriptorSet* 							descriptor_set;
            int32_t									width;
            int32_t									height;

			void createInstance()
			{
				std::vector<const char *> _layer_names = this->getLayerNames();
				std::vector<const char *> _instance_extension_names;

				_instance_extension_names.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
				//@TODO extension for win32 VK_KHR_WIN32_SURFACE_EXTENSION_NAME
				_instance_extension_names.push_back("VK_KHR_xcb_surface");

				VkApplicationInfo _app_info = {};
				_app_info.sType 				= VK_STRUCTURE_TYPE_APPLICATION_INFO;
				_app_info.pNext 				= NULL;
				_app_info.pApplicationName 		= APP_NAME;
				_app_info.applicationVersion 	= 1;
				_app_info.pEngineName 			= APP_NAME;
				_app_info.engineVersion 		= 1;
				_app_info.apiVersion 			= VK_API_VERSION_1_0;

				VkInstanceCreateInfo _inst_info = {};
				_inst_info.sType 					= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
				_inst_info.pNext 					= NULL;
				_inst_info.flags 					= 0;
				_inst_info.pApplicationInfo 		= &_app_info;
				_inst_info.enabledLayerCount 		= (uint32_t) _layer_names.size();
				_inst_info.ppEnabledLayerNames 		= _layer_names.size() ? _layer_names.data() : NULL;
				_inst_info.enabledExtensionCount 	= (uint32_t) _instance_extension_names.size();
				_inst_info.ppEnabledExtensionNames 	= _instance_extension_names.data();

				VkResult res = vkCreateInstance(&_inst_info, NULL, &instance);
				assert(res == VK_SUCCESS);
			}

			void createLogicalDeviceAndCommandBuffer()
			{

				VkResult res = vkEnumeratePhysicalDevices(instance, &queue_family_count, NULL);
				assert(res == VK_SUCCESS && queue_family_count);
				gpu_vector.resize(queue_family_count);
				res = vkEnumeratePhysicalDevices(instance, &queue_family_count, gpu_vector.data());
				assert(res == VK_SUCCESS);

				vkGetPhysicalDeviceQueueFamilyProperties(gpu_vector[0], &queue_family_count, NULL);
				assert(queue_family_count >= 1);

				queue_family_props.resize(queue_family_count);
				vkGetPhysicalDeviceQueueFamilyProperties(gpu_vector[0], &queue_family_count, queue_family_props.data());
				assert(queue_family_count >= 1);

				vkGetPhysicalDeviceMemoryProperties(gpu_vector[0], &memory_properties);
				vkGetPhysicalDeviceProperties(gpu_vector[0], &gpu_props);

				VkDeviceQueueCreateInfo queue_info = {};

				bool found = false;
				for (unsigned int i = 0; i < queue_family_count; i++) {
					if (queue_family_props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
						queue_info.queueFamilyIndex = i;
						found = true;
						break;
					}
				}
				assert(found);

				float queue_priorities[1] = {0.0};
				queue_info.sType 			= VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queue_info.pNext 			= NULL;
				queue_info.queueCount 		= 1;
				queue_info.pQueuePriorities = queue_priorities;

				std::vector<const char *> device_extension_names;
				device_extension_names.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

				VkDeviceCreateInfo device_info = {};
				device_info.sType 					= VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
				device_info.pNext 					= NULL;
				device_info.queueCreateInfoCount 	= 1;
				device_info.pQueueCreateInfos 		= &queue_info;
				device_info.enabledExtensionCount 	= (uint32_t)device_extension_names.size();
				device_info.ppEnabledExtensionNames = device_info.enabledExtensionCount ? device_extension_names.data() : NULL;
				device_info.enabledLayerCount 		= 0;
				device_info.ppEnabledLayerNames 	= NULL;
				device_info.pEnabledFeatures 		= NULL;

				res = vkCreateDevice(gpu_vector[0], &device_info, NULL, &device);
				assert(res == VK_SUCCESS);

				VkCommandPoolCreateInfo cmd_pool_info = {};
				cmd_pool_info.sType 			= VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
				cmd_pool_info.pNext 			= NULL;
				cmd_pool_info.queueFamilyIndex  = queue_info.queueFamilyIndex;
				cmd_pool_info.flags 			= 0;

				res = vkCreateCommandPool(device, &cmd_pool_info, NULL, &_command_pool);
				assert(res == VK_SUCCESS);

				command_buffer.resize(3);

				/* Create the command buffer from the command pool */
				VkCommandBufferAllocateInfo cmd = {};
				cmd.sType 				= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				cmd.pNext 			 	= NULL;
				cmd.commandPool 	 	= _command_pool;
				cmd.level 			 	= VK_COMMAND_BUFFER_LEVEL_PRIMARY;
				cmd.commandBufferCount  = 3;

				res = vkAllocateCommandBuffers(device, &cmd, command_buffer.data());
				assert(res == VK_SUCCESS);
			}

			void initGraphicPipeline (const bool depthPresent)
			{
				VkResult res;

				/* Render Pass */

				struct SwapChainParams sc_params = {};
				sc_params.gpu 					= gpu_vector[0];
				sc_params.width 				= static_cast<u_int32_t >(width);
				sc_params.height 				= static_cast<u_int32_t >(height);
				sc_params.device 				= device;
				sc_params.queue_family_count 	= queue_family_count;
				sc_params.queue_family_props 	= queue_family_props;
				sc_params.surface 				= surface;
				sc_params.memory_props 			= memory_properties;
				render_pass = new RenderPass(device, sc_params);

				std::vector< struct rpAttachments > rp_attachments = {};
				struct rpAttachments attch = {};
				/* Color Attach */
				attch.format = render_pass->getSwapChain()->getSwapChainFormat();
				attch.clear  = true;
				rp_attachments.push_back(attch);
				/* Depth Attach */
				attch.format = render_pass->getDepthBufferFormat();
				attch.clear  = true;
				rp_attachments.push_back(attch);

				render_pass->create(rp_attachments);

				/* Descriptor Set */

				descriptor_set = new DescriptorSet(device);

				struct DescriptorSetParams ds_params = {};
				ds_params.width 				= static_cast<u_int32_t >(width);
				ds_params.height 				= static_cast<u_int32_t >(height);
				ds_params.memory_properties		= memory_properties;
				ds_params.command_pool			= _command_pool;
				ds_params.gpu					= gpu_vector[0];
				ds_params.graphic_queue			= render_pass->getSwapChain()->getGraphicQueue();

				descriptor_set->create(ds_params);


				/* Vertex Buffer */

				struct BufferData vertexBufferData = {};

				vertexBufferData.device            = device;
				vertexBufferData.usage             = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
				vertexBufferData.physicalDevice    = gpu_vector[0];
				vertexBufferData.properties        = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
				vertexBufferData.size              = vertexData.size() * sizeof(Vertex);

				vertex_buffer = new VertexBuffer(vertexBufferData);


                GraphicPipeline *_graphic_pipeline = new GraphicPipeline(device);
                _graphic_pipeline->create(descriptor_set->getPipelineLayout(), render_pass->getRenderPass());

				VkSemaphoreCreateInfo imageAcquiredSemaphoreCreateInfo;
				imageAcquiredSemaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
				imageAcquiredSemaphoreCreateInfo.pNext = NULL;
				imageAcquiredSemaphoreCreateInfo.flags = 0;

				res = vkCreateSemaphore(device, &imageAcquiredSemaphoreCreateInfo, NULL, &imageAcquiredSemaphore);
				assert(res == VK_SUCCESS);

				res = vkCreateSemaphore(device, &imageAcquiredSemaphoreCreateInfo, NULL, &renderSemaphore);
				assert(res == VK_SUCCESS);


				VkClearValue clear_values[2];
				clear_values[0].color.float32[0] = 0.2f;
				clear_values[0].color.float32[1] = 0.2f;
				clear_values[0].color.float32[2] = 0.2f;
				clear_values[0].color.float32[3] = 0.2f;
				clear_values[1].depthStencil.depth = 1.0f;
				clear_values[1].depthStencil.stencil = 0;

				VkRenderPassBeginInfo rp_begin;
				rp_begin.sType 											= VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
				rp_begin.pNext 											= NULL;
				rp_begin.renderPass 									= render_pass->getRenderPass();
				rp_begin.renderArea.offset.x 							= 0;
				rp_begin.renderArea.offset.y 							= 0;
				rp_begin.renderArea.extent.width 						= (uint32_t)width;
				rp_begin.renderArea.extent.height 						= (uint32_t)height;
				rp_begin.clearValueCount 								= 2;
				rp_begin.pClearValues 									= clear_values;

				VkFenceCreateInfo fenceInfo;
				fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
				fenceInfo.pNext = NULL;
				fenceInfo.flags = 0;

				VkCommandBufferBeginInfo cmd_buf_info = {};
				cmd_buf_info.sType 							= VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				cmd_buf_info.pNext 							= NULL;
				cmd_buf_info.flags 							= 0;
				cmd_buf_info.pInheritanceInfo 				= NULL;

				drawFence.resize(command_buffer.size());
				for (u_int32_t i = 0; i < command_buffer.size(); i++){
					rp_begin.framebuffer = render_pass->getFrameBuffer()[i];

					res = vkBeginCommandBuffer(command_buffer[i], &cmd_buf_info);
					assert(res == VK_SUCCESS);

					vkCreateFence(device, &fenceInfo, NULL, &drawFence[i]);

					vkCmdBeginRenderPass(command_buffer[i], &rp_begin, VK_SUBPASS_CONTENTS_INLINE);
					vkCmdBindPipeline(command_buffer[i], VK_PIPELINE_BIND_POINT_GRAPHICS, _graphic_pipeline->getPipeline());
					vkCmdBindDescriptorSets(command_buffer[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
											descriptor_set->getPipelineLayout(), 0, 1, descriptor_set->getDescriptorSet(), 0, NULL);

					const VkDeviceSize offsets[1] = {0};

					//std::vector<VkBuffer> buffers = VertexBuffer::getBuffersFromVector(vertex_buffer);
					vkCmdBindVertexBuffers(command_buffer[i], 0, 1, &vertex_buffer->buf, offsets);

					Util::init_viewports(command_buffer[i], width, height);
					Util::init_scissors(command_buffer[i], static_cast<u_int32_t>(width), static_cast<u_int32_t>(height));

					vkCmdDraw(command_buffer[i], static_cast<uint32_t>(vertexData.size()), 1, 0, 0);
					vkCmdEndRenderPass(command_buffer[i]);
					res = vkEndCommandBuffer(command_buffer[i]);
					assert(res == VK_SUCCESS);

				}
			}
		};
	}
}
#endif //OBSIDIAN2D_CORE_WINDOW2_H