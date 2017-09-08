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
#include "SyncPrimitives.h"
#include "CommandBuffers.h"

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

				vkDestroyPipeline(device, vkPipeline, NULL);
				vkDestroyPipelineCache(device, pPipelineCache, NULL);

				delete descriptor_set;
				delete vertex_buffer;

				// Destroy shaders
				vkDestroyShaderModule(device, shaderStages[0].module, NULL);
				vkDestroyShaderModule(device, shaderStages[1].module, NULL);

				//vkFreeCommandBuffers(device, _command_pool, (u_int32_t)command_buffer.size(), command_buffer.data());

				vkDestroySemaphore(device, imageAcquiredSemaphore, nullptr);
				vkDestroySemaphore(device, renderSemaphore, nullptr);

				//for(i = 0; i < drawFence.size(); i++){
				//	vkDestroyFence(device, drawFence[i], nullptr);
				//}

				vkFreeMemory(device, Textures::textureImageMemory, nullptr);

				//vkDestroyCommandPool(device, _command_pool, NULL);
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

				current_buffer = 0;

				VkPipelineStageFlags pipe_stage_flags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

				VkSubmitInfo submit_info;
				submit_info.pNext                     = NULL;
				submit_info.sType                     = VK_STRUCTURE_TYPE_SUBMIT_INFO;
				submit_info.waitSemaphoreCount        = 1;
				submit_info.pWaitSemaphores           = &imageAcquiredSemaphore;
				submit_info.pWaitDstStageMask         = &pipe_stage_flags;
				submit_info.commandBufferCount        = 1;
				submit_info.pCommandBuffers           = command_buffer[current_buffer]->getCommandBuffer();
				submit_info.signalSemaphoreCount      = 1;
				submit_info.pSignalSemaphores         = &renderSemaphore;

				res = vkQueueSubmit(render_pass->getSwapChain()->getGraphicQueue(), 1, &submit_info, *sync_primitives->getFence(current_buffer));
				assert(res == VK_SUCCESS);

				do {
					res = vkWaitForFences(device, 1, sync_primitives->getFence(current_buffer), VK_TRUE, VK_SAMPLE_COUNT_1_BIT);
				} while (res == VK_TIMEOUT);
				assert(res == VK_SUCCESS);
				vkResetFences(device, 1, sync_primitives->getFence(current_buffer));

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
			VkPipelineShaderStageCreateInfo 		shaderStages[2];
			VkDevice 								device;
			std::vector<CommandBuffers *>			command_buffer;
			VkPhysicalDeviceMemoryProperties 		memory_properties;
			uint32_t 								current_buffer = 0;
			std::vector<VkPhysicalDevice> 			gpu_vector;
			u_int32_t							 	queue_family_count;
			std::vector<VkQueueFamilyProperties> 	queue_family_props;
			VkPipeline 								vkPipeline;
			VkPipelineCache 						pPipelineCache;


			SyncPrimitives* 						sync_primitives;
			RenderPass* 							render_pass;
		protected:
			VertexBuffer *             				vertex_buffer;
			DescriptorSet* 							descriptor_set;

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


				bool found = false;
				u_int32_t queueFamilyIndex = UINT_MAX;
				for (unsigned int i = 0; i < queue_family_count; i++) {
					if (queue_family_props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
						queueFamilyIndex = i;
						found = true;
						break;
					}
				}
				assert(found && queueFamilyIndex != UINT_MAX);

				float queue_priorities[1] = {0.0};

				VkDeviceQueueCreateInfo queue_info = {};
				queue_info.sType 			= VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queue_info.pNext 			= NULL;
				queue_info.queueCount 		= 1;
				queue_info.pQueuePriorities = queue_priorities;
				queue_info.queueFamilyIndex = queueFamilyIndex;

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

				/* Push Command Buffer */
				command_buffer.push_back( new CommandBuffers(device, queueFamilyIndex) );

/*				VkCommandPoolCreateInfo cmd_pool_info = {};
				cmd_pool_info.sType 			= VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
				cmd_pool_info.pNext 			= NULL;
				cmd_pool_info.queueFamilyIndex  = queueFamilyIndex;
				cmd_pool_info.flags 			= 0;

				res = vkCreateCommandPool(device, &cmd_pool_info, NULL, &_command_pool);
				assert(res == VK_SUCCESS);

				command_buffer.resize(3);

				VkCommandBufferAllocateInfo cmd = {};
				cmd.sType 				= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				cmd.pNext 			 	= NULL;
				cmd.commandPool 	 	= _command_pool;
				cmd.level 			 	= VK_COMMAND_BUFFER_LEVEL_PRIMARY;
				cmd.commandBufferCount  = 3;

				res = vkAllocateCommandBuffers(device, &cmd, command_buffer.data());
				assert(res == VK_SUCCESS);*/
			}

			void initGraphicPipeline (const bool depthPresent)
			{
				VkResult U_ASSERT_ONLY 	res;

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
				ds_params.command_pool			= command_buffer[0]->getCommandPool();
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


				// Vertex shader
				shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				// Set pipeline stage for this shader
				shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
				// Load binary SPIR-V shader
				shaderStages[0].module = loadSPIRVShader(getAssetPath() + "shaders/triangle/vert.spv", device);
				// Main entry point for the shader
				shaderStages[0].pName = "main";
				assert(shaderStages[0].module != VK_NULL_HANDLE);

				// Fragment shader
				shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				// Set pipeline stage for this shader
				shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
				// Load binary SPIR-V shader
				shaderStages[1].module = loadSPIRVShader(getAssetPath() + "shaders/triangle/frag.spv", device);
				// Main entry point for the shader
				shaderStages[1].pName = "main";
				assert(shaderStages[1].module != VK_NULL_HANDLE);


				VkVertexInputBindingDescription vi_binding;
				vi_binding.binding 										= 0;
				vi_binding.inputRate 									= VK_VERTEX_INPUT_RATE_VERTEX;
				vi_binding.stride 										= sizeof(Vertex);

				std::vector<VkVertexInputAttributeDescription> vi_attribs;
				vi_attribs.resize(3);

				vi_attribs[0].binding 									= 0;
				vi_attribs[0].location 									= 0;
				vi_attribs[0].format 									= VK_FORMAT_R32G32B32_SFLOAT;
				vi_attribs[0].offset 									= offsetof(Vertex, pos);

				vi_attribs[1].binding 									= 0;
				vi_attribs[1].location 									= 1;
				vi_attribs[1].format 									= VK_FORMAT_R32G32_SFLOAT;
				vi_attribs[1].offset 									= offsetof(Vertex, uv);

				vi_attribs[2].binding 									= 0;
				vi_attribs[2].location 									= 2;
				vi_attribs[2].format 									= VK_FORMAT_R32G32B32_SFLOAT;
				vi_attribs[2].offset 									= offsetof(Vertex, normal);

				VkPipelineCacheCreateInfo pipelineCache;
				pipelineCache.sType 									= VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
				pipelineCache.pNext 									= NULL;
				pipelineCache.initialDataSize 							= 0;
				pipelineCache.pInitialData 								= NULL;
				pipelineCache.flags 									= 0;
				res = vkCreatePipelineCache(device, &pipelineCache, NULL, &pPipelineCache);
				assert(res == VK_SUCCESS);


				VkDynamicState dynamicStateEnables[VK_DYNAMIC_STATE_RANGE_SIZE];
				VkPipelineDynamicStateCreateInfo dynamicState = {};
				memset(dynamicStateEnables, 0, sizeof dynamicStateEnables);

				dynamicState.sType 										= VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
				dynamicState.pNext 										= NULL;
				dynamicState.pDynamicStates 							= dynamicStateEnables;
				dynamicState.dynamicStateCount 							= 0;

				bool include_vi = true;

				VkPipelineVertexInputStateCreateInfo vi;
				memset(&vi, 0, sizeof(vi));
				vi.sType 												= VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
				if (include_vi) {
					vi.pNext 											= NULL;
					vi.flags 											= 0;
					vi.vertexBindingDescriptionCount 					= 1;
					vi.pVertexBindingDescriptions 						= &vi_binding;
					vi.vertexAttributeDescriptionCount 					= 3;
					vi.pVertexAttributeDescriptions 					= vi_attribs.data();
				}
				VkPipelineInputAssemblyStateCreateInfo ia;
				ia.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
				ia.pNext = NULL;
				ia.flags = 0;
				ia.primitiveRestartEnable = VK_FALSE;
				ia.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

				VkPipelineRasterizationStateCreateInfo rs;
				rs.sType 												= VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
				rs.pNext 												= NULL;
				rs.flags 												= 0;
				rs.polygonMode 											= VK_POLYGON_MODE_FILL;
				rs.cullMode 											= VK_CULL_MODE_BACK_BIT;
				rs.frontFace 											= VK_FRONT_FACE_CLOCKWISE;
				rs.depthClampEnable 									= VK_FALSE;
				rs.rasterizerDiscardEnable 								= VK_FALSE;
				rs.depthBiasEnable 										= VK_FALSE;
				rs.depthBiasConstantFactor 								= 0;
				rs.depthBiasClamp 										= 0;
				rs.depthBiasSlopeFactor 								= 0;
				rs.lineWidth 											= 1.0f;

				VkPipelineColorBlendStateCreateInfo cb;
				cb.sType 												= VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
				cb.flags 												= 0;
				cb.pNext 												= NULL;

				VkPipelineColorBlendAttachmentState att_state[1];
				att_state[0].colorWriteMask 							= 0xf;
				att_state[0].blendEnable 								= VK_FALSE;
				att_state[0].alphaBlendOp 								= VK_BLEND_OP_ADD;
				att_state[0].colorBlendOp 								= VK_BLEND_OP_ADD;
				att_state[0].srcColorBlendFactor 						= VK_BLEND_FACTOR_ZERO;
				att_state[0].dstColorBlendFactor 						= VK_BLEND_FACTOR_ZERO;
				att_state[0].srcAlphaBlendFactor 						= VK_BLEND_FACTOR_ZERO;
				att_state[0].dstAlphaBlendFactor 						= VK_BLEND_FACTOR_ZERO;
				cb.attachmentCount 										= 1;
				cb.pAttachments 										= att_state;
				cb.logicOpEnable 										= VK_FALSE;
				cb.logicOp 												= VK_LOGIC_OP_NO_OP;
				cb.blendConstants[0] 									= 1.0f;
				cb.blendConstants[1] 									= 1.0f;
				cb.blendConstants[2] 									= 1.0f;
				cb.blendConstants[3] 									= 1.0f;

				VkPipelineViewportStateCreateInfo vp = {};
				vp.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
				vp.pNext = NULL;
				vp.flags = 0;
				vp.viewportCount 										= 1;
				dynamicStateEnables[dynamicState.dynamicStateCount++] 	= VK_DYNAMIC_STATE_VIEWPORT;
				vp.scissorCount 										= 1;
				dynamicStateEnables[dynamicState.dynamicStateCount++] 	= VK_DYNAMIC_STATE_SCISSOR;
				vp.pScissors 											= NULL;
				vp.pViewports 											= NULL;

				VkPipelineDepthStencilStateCreateInfo ds;
				ds.sType 												= VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
				ds.pNext 												= NULL;
				ds.flags 												= 0;
				ds.depthTestEnable 										= (VkBool32) depthPresent;
				ds.depthWriteEnable 									= (VkBool32) depthPresent;
				ds.depthCompareOp 										= VK_COMPARE_OP_LESS_OR_EQUAL;
				ds.depthBoundsTestEnable 								= VK_FALSE;
				ds.stencilTestEnable 									= VK_FALSE;
				ds.back.failOp 											= VK_STENCIL_OP_KEEP;
				ds.back.passOp 											= VK_STENCIL_OP_KEEP;
				ds.back.compareOp 										= VK_COMPARE_OP_ALWAYS;
				ds.back.compareMask 									= 0;
				ds.back.reference 										= 0;
				ds.back.depthFailOp 									= VK_STENCIL_OP_KEEP;
				ds.back.writeMask 										= 0;
				ds.minDepthBounds 										= 0;
				ds.maxDepthBounds 										= 0;
				ds.stencilTestEnable 									= VK_FALSE;
				ds.front 												= ds.back;

				VkPipelineMultisampleStateCreateInfo ms;
				ms.sType 														= VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
				ms.pNext 														= NULL;
				ms.flags 														= 0;
				ms.pSampleMask 													= NULL;
				ms.rasterizationSamples 										= VK_SAMPLE_COUNT_1_BIT;
				ms.sampleShadingEnable 											= VK_FALSE;
				ms.alphaToCoverageEnable 										= VK_FALSE;
				ms.alphaToOneEnable 											= VK_FALSE;
				ms.minSampleShading 											= 0.0;

				VkGraphicsPipelineCreateInfo pipeline;
				pipeline.sType 													= VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
				pipeline.pNext 													= NULL;
				pipeline.layout 												= descriptor_set->getPipelineLayout();
				pipeline.basePipelineHandle 									= VK_NULL_HANDLE;
				pipeline.basePipelineIndex 										= 0;
				pipeline.flags 													= 0;
				pipeline.pVertexInputState 										= &vi;
				pipeline.pInputAssemblyState 									= &ia;
				pipeline.pRasterizationState 									= &rs;
				pipeline.pColorBlendState 										= &cb;
				pipeline.pTessellationState 									= NULL;
				pipeline.pMultisampleState 										= &ms;
				pipeline.pDynamicState 											= &dynamicState;
				pipeline.pViewportState 										= &vp;
				pipeline.pDepthStencilState 									= &ds;
				pipeline.pStages 												= shaderStages;
				pipeline.stageCount 											= 2;
				pipeline.renderPass 											= render_pass->getRenderPass();
				pipeline.subpass 												= 0;

				res = vkCreateGraphicsPipelines(device, pPipelineCache, 1, &pipeline, NULL, &vkPipeline);
				assert(res == VK_SUCCESS);



				sync_primitives = new SyncPrimitives(device);
				sync_primitives->createFence();


				VkSemaphoreCreateInfo imageAcquiredSemaphoreCreateInfo;
				imageAcquiredSemaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
				imageAcquiredSemaphoreCreateInfo.pNext = NULL;
				imageAcquiredSemaphoreCreateInfo.flags = 0;

				res = vkCreateSemaphore(device, &imageAcquiredSemaphoreCreateInfo, NULL, &imageAcquiredSemaphore);
				assert(res == VK_SUCCESS);

				res = vkCreateSemaphore(device, &imageAcquiredSemaphoreCreateInfo, NULL, &renderSemaphore);
				assert(res == VK_SUCCESS);

				command_buffer[command_buffer.size()-1]->bindCommandBuffer(
						render_pass,
						descriptor_set,
						vkPipeline,
						current_buffer,
						static_cast<uint32_t>(width),
						static_cast<uint32_t>(height),
						sync_primitives,
						vertex_buffer
				);
				/*
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
					vkCmdBindPipeline(command_buffer[i], VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipeline);
					vkCmdBindDescriptorSets(command_buffer[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
											descriptor_set->getPipelineLayout(), 0, 1, descriptor_set->getDescriptorSet(), 0, NULL);

					const VkDeviceSize offsets[1] = {0};

					//std::vector<VkBuffer> buffers = VertexBuffer::getBuffersFromVector(vertex_buffer);
					vkCmdBindVertexBuffers(command_buffer[i], 0, 1, &vertex_buffer->buf, offsets);

					this->init_viewports(command_buffer[i]);
					this->init_scissors(command_buffer[i]);

					vkCmdDraw(command_buffer[i], static_cast<uint32_t>(vertexData.size()), 1, 0, 0);
					vkCmdEndRenderPass(command_buffer[i]);
					res = vkEndCommandBuffer(command_buffer[i]);
					assert(res == VK_SUCCESS);

				}*/
			}
		};
	}
}
#endif //OBSIDIAN2D_CORE_WINDOW2_H