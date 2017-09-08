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

                delete descriptor_set;
                delete graphic_pipeline;
                delete vertex_buffer;
                delete sync_primitives;

                for (i = 0; i < command_buffer.size(); i++)
                {
                    delete command_buffer[i];
                }

                vkDestroySemaphore(device, imageAcquiredSemaphore, nullptr);
                vkDestroySemaphore(device, renderSemaphore, nullptr);

                vkFreeMemory(device, Textures::textureImageMemory, nullptr);

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
            VkDevice 								device;
            VkPhysicalDeviceMemoryProperties 		memory_properties;
            uint32_t 								current_buffer = 0;
            std::vector<VkPhysicalDevice> 			gpu_vector;
            u_int32_t							 	queue_family_count;
            std::vector<VkQueueFamilyProperties> 	queue_family_props;

            GraphicPipeline*                        graphic_pipeline;
            std::vector<CommandBuffers *>			command_buffer;
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
            }

            void initGraphicPipeline ()
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
                attch.format = render_pass->getSwapChain()->getSwapChainFormat();
                attch.clear  = true;
                rp_attachments.push_back(attch);
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


                graphic_pipeline = new GraphicPipeline(device);
                graphic_pipeline->create(descriptor_set->getPipelineLayout(), render_pass->getRenderPass());


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

                command_buffer[command_buffer.size()-1]
                        ->bindCommandBuffer(
                                render_pass,
                                descriptor_set,
                                graphic_pipeline->getPipeline(),
                                current_buffer,
                                static_cast<uint32_t>(width),
                                static_cast<uint32_t>(height),
                                sync_primitives,
                                vertex_buffer
                        );
            }
        };
    }
}
#endif //OBSIDIAN2D_CORE_WINDOW2_H