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

                vkDestroyImage(device, texture_image, nullptr);
                vkDestroyImageView(device,texture_image_view, nullptr);
                vkDestroySampler(device, texture_sampler, nullptr);

                if (surface != VK_NULL_HANDLE)
                {
                    vkDestroySurfaceKHR(instance, surface, nullptr);
                }

				vkDestroyPipeline(device, vkPipeline, NULL);
				vkDestroyPipelineCache(device, pPipelineCache, NULL);
				vkDestroyDescriptorPool(device, desc_pool, NULL);

				// Destroy buffers

				for(auto v_buff : vertex_buffer)
				{
					delete v_buff;
				}
				delete uniform_buffer;

				// Destroy shaders
				vkDestroyShaderModule(device, shaderStages[0].module, NULL);
				vkDestroyShaderModule(device, shaderStages[1].module, NULL);

				// Destroy render pass
				//vkDestroyRenderPass(device, render_pass, NULL);

				// Destroy descriptor and pipeline layouts
				for (i = 0; i < 1; i++) vkDestroyDescriptorSetLayout(device, desc_layout[i], NULL);
				vkDestroyPipelineLayout(device, pipeline_layout, NULL);

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

                uniform_buffer->updateCamera(device);

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
			VkPipelineShaderStageCreateInfo 		shaderStages[2];
			std::vector<VkDescriptorSetLayout> 		desc_layout;
			VkPipelineLayout 						pipeline_layout;
			VkCommandPool							_command_pool;
			VkDevice 								device;
			std::vector<VkCommandBuffer>			command_buffer;
			VkPhysicalDeviceMemoryProperties 		memory_properties;
			uint32_t 								current_buffer = 0;
			std::vector<VkPhysicalDevice> 			gpu_vector;
			u_int32_t							 	queue_family_count;
			std::vector<VkQueueFamilyProperties> 	queue_family_props;
			VkPipeline 								vkPipeline;
			VkPipelineCache 						pPipelineCache;
			VkDescriptorPool 						desc_pool;
            VkSampler                               texture_sampler = nullptr;
            VkImage                                 texture_image = nullptr;
            VkImageView                             texture_image_view = nullptr;
			RenderPass* render_pass;
		protected:
            std::vector<VertexBuffer *>             vertex_buffer;
            UniformBuffer*                          uniform_buffer;

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

			void createVertexBuffer(std::vector<Vertex> _vertexData)
			{
				struct BufferData vertexBufferData = {};

				vertexBufferData.device            = device;
				vertexBufferData.usage             = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
				vertexBufferData.physicalDevice    = gpu_vector[0];
				vertexBufferData.properties        = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
				vertexBufferData.size              = _vertexData.size() * sizeof(Vertex);

				vertex_buffer.push_back(new VertexBuffer(vertexBufferData));
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


				/*  create Uniform Buffer  */

                struct BufferData uniformBufferData = {};

                uniformBufferData.device            = device;
                uniformBufferData.usage             = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
                uniformBufferData.physicalDevice    = gpu_vector[0];
                uniformBufferData.properties        = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
                uniformBufferData.size              = sizeof(glm::mat4);

                uniform_buffer = new UniformBuffer(uniformBufferData);
                uniform_buffer->initModelView(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
                glm::mat4 MVP = uniform_buffer->getMVP();



				bool use_texture = true;
				std::vector<VkDescriptorSetLayoutBinding>layout_bindings = {};
                layout_bindings.resize(2);
				layout_bindings[0].binding 								= 0;
				layout_bindings[0].descriptorType 						= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				layout_bindings[0].descriptorCount 						= 1;
				layout_bindings[0].stageFlags 							= VK_SHADER_STAGE_VERTEX_BIT;
				layout_bindings[0].pImmutableSamplers					= NULL;

				if (use_texture) {
					layout_bindings[1].binding 							= 1;
					layout_bindings[1].descriptorType 					= VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					layout_bindings[1].descriptorCount 					= 1;
					layout_bindings[1].stageFlags 						= VK_SHADER_STAGE_FRAGMENT_BIT;
					layout_bindings[1].pImmutableSamplers 				= NULL;
				}

				/* Next take layout bindings and use them to create a descriptor set layout
				 */
				VkDescriptorSetLayoutCreateInfo descriptor_layout = {};
				descriptor_layout.sType 								= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
				descriptor_layout.pNext 								= NULL;
				descriptor_layout.bindingCount 							= use_texture ? 2 : 1;
				descriptor_layout.pBindings 							= layout_bindings.data();

				desc_layout.resize(1);
				res = vkCreateDescriptorSetLayout(device, &descriptor_layout, NULL, desc_layout.data());
				assert(res == VK_SUCCESS);

				/* Now use the descriptor layout to create a pipeline layout */
				VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo = {};
				pPipelineLayoutCreateInfo.sType                         = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
				pPipelineLayoutCreateInfo.pNext                         = NULL;
				pPipelineLayoutCreateInfo.pushConstantRangeCount        = 0;
				pPipelineLayoutCreateInfo.pPushConstantRanges           = NULL;
				pPipelineLayoutCreateInfo.setLayoutCount                = 1;
				pPipelineLayoutCreateInfo.pSetLayouts                   = desc_layout.data();

				res = vkCreatePipelineLayout(device, &pPipelineLayoutCreateInfo, NULL, &pipeline_layout);
				assert(res == VK_SUCCESS);




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

                this->createVertexBuffer(vertexData);

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

				VkDescriptorPoolSize type_count[2];
				type_count[0].type 										= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				type_count[0].descriptorCount 							= 1;
				if (use_texture) {
					type_count[1].type 									= VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					type_count[1].descriptorCount 						= 1;
				}

				VkDescriptorPoolCreateInfo descriptor_pool = {};
				descriptor_pool.sType 									= VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
				descriptor_pool.pNext 									= NULL;
				descriptor_pool.maxSets 								= 1;
				descriptor_pool.poolSizeCount 							= use_texture ? 2 : 1;
				descriptor_pool.pPoolSizes 								= type_count;



				res = vkCreateDescriptorPool(device, &descriptor_pool, NULL, &desc_pool);
				assert(res == VK_SUCCESS);

				VkDescriptorSetAllocateInfo _alloc_info[1];
				_alloc_info[0].sType 									= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
				_alloc_info[0].pNext 									= NULL;
				_alloc_info[0].descriptorPool 							= desc_pool;
				_alloc_info[0].descriptorSetCount 						= 1;
				_alloc_info[0].pSetLayouts 								= desc_layout.data();

                VkDescriptorSet desc_set;
				res = vkAllocateDescriptorSets(device, _alloc_info, &desc_set);
				assert(res == VK_SUCCESS);

				texture_image = Textures::createTextureImage(gpu_vector[0], device, "../../include/Obsidian2D/Renderer/shaders/baleog.jpg",
															 _command_pool, render_pass->getSwapChain()->getGraphicQueue(), memory_properties);
				texture_image_view = Textures::createImageView(device, texture_image, VK_FORMAT_R8G8B8A8_UNORM);

                VkSamplerCreateInfo sampler = {};
				sampler.sType 											= VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
				sampler.maxAnisotropy 									= 1.0f;
				sampler.magFilter 										= VK_FILTER_LINEAR;
				sampler.minFilter 										= VK_FILTER_LINEAR;
				sampler.mipmapMode 										= VK_SAMPLER_MIPMAP_MODE_LINEAR;
				sampler.addressModeU 									= VK_SAMPLER_ADDRESS_MODE_REPEAT;
				sampler.addressModeV 									= VK_SAMPLER_ADDRESS_MODE_REPEAT;
				sampler.addressModeW 									= VK_SAMPLER_ADDRESS_MODE_REPEAT;
				sampler.mipLodBias 										= 0.0f;
				sampler.compareOp 										= VK_COMPARE_OP_NEVER;
				sampler.minLod 											= 0.0f;
				sampler.maxLod 											= 0.0f;
				sampler.maxAnisotropy 									= 1.0;
				sampler.anisotropyEnable 								= VK_FALSE;
				sampler.borderColor 									= VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;

                assert(vkCreateSampler(device, &sampler, nullptr, &texture_sampler) == VK_SUCCESS);

				VkDescriptorImageInfo texture_info;
				texture_info.imageLayout 								= VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				texture_info.imageView 									= texture_image_view;
				texture_info.sampler 									= texture_sampler;

                VkWriteDescriptorSet writes[2];
                writes[0] = {};
				writes[0].sType 										= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				writes[0].pNext 										= NULL;
				writes[0].dstSet 										= desc_set;
				writes[0].descriptorCount 								= 1;
				writes[0].descriptorType 								= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				writes[0].pBufferInfo 									= &uniform_buffer->buffer_info;
				writes[0].dstArrayElement 								= 0;
				writes[0].dstBinding 									= 0;

				writes[1] = {};
				writes[1].sType 										= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				writes[1].dstSet 										= desc_set;
				writes[1].dstBinding 									= 1;
				writes[1].descriptorCount 								= 1;
				writes[1].descriptorType 								= VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				writes[1].pImageInfo 									= &texture_info;
				writes[1].dstArrayElement 								= 0;

				vkUpdateDescriptorSets(device, 2, writes, 0, NULL);

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
#ifndef __ANDROID__
				vp.viewportCount 										= 1;
				dynamicStateEnables[dynamicState.dynamicStateCount++] 	= VK_DYNAMIC_STATE_VIEWPORT;
				vp.scissorCount 										= 1;
				dynamicStateEnables[dynamicState.dynamicStateCount++] 	= VK_DYNAMIC_STATE_SCISSOR;
				vp.pScissors 											= NULL;
				vp.pViewports 											= NULL;
#else
				// Temporary disabling dynamic viewport on Android because some of drivers doesn't
    // support the feature.
    VkViewport viewports;
    viewports.minDepth = 0.0f;
    viewports.maxDepth = 1.0f;
    viewports.x = 0;
    viewports.y = 0;
    viewports.width = info.width;
    viewports.height = info.height;
    VkRect2D scissor;
    scissor.extent.width = info.width;
    scissor.extent.height = info.height;
    scissor.offset.x = 0;
    scissor.offset.y = 0;
    vp.viewportCount = NUM_VIEWPORTS;
    vp.scissorCount = NUM_SCISSORS;
    vp.pScissors = &scissor;
    vp.pViewports = &viewports;
#endif
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
				pipeline.layout 												= pipeline_layout;
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
					vkCmdBindPipeline(command_buffer[i], VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipeline);
					vkCmdBindDescriptorSets(command_buffer[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
                                            pipeline_layout, 0, 1, &desc_set, 0, NULL);

					const VkDeviceSize offsets[1] = {0};

					std::vector<VkBuffer> buffers = VertexBuffer::getBuffersFromVector(vertex_buffer);
					vkCmdBindVertexBuffers(command_buffer[i], 0, static_cast<uint32_t >(buffers.size()), buffers.data(), offsets);

					this->init_viewports(command_buffer[i]);
					this->init_scissors(command_buffer[i]);

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
