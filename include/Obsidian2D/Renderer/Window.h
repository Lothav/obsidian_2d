#ifndef OBSIDIAN2D_RENDERER_WINDOW_
#define OBSIDIAN2D_RENDERER_WINDOW_

#include <malloc.h>
#include <assert.h>

#include <string>

#include "Obsidian2D/Renderer/VulkanInfo.h"

#include "Obsidian2D/Util/Loggable.h"

#define APP_NAME "Obsidian2D"

namespace Obsidian2D
{
	namespace Renderer
	{
		class Window : public Loggable
		{
		protected:
			struct VulkanInfo info = {};
			VkDevice device;

			VkResult setGlobalLayerProperties(struct VulkanInfo &info)
			{
				uint32_t instance_layer_count;
				VkLayerProperties *vk_props = NULL;
				VkResult res;
			
			#ifdef __ANDROID__
				if (!InitVulkan()) {
					LOGE("Failied initializing Vulkan APIs!");
					return VK_ERROR_INITIALIZATION_FAILED;
				}
				LOGI("Loaded Vulkan APIs.");
			#endif

				do {
					res = vkEnumerateInstanceLayerProperties(&instance_layer_count, NULL);
					if (res) return res;
					if (instance_layer_count == 0) return VK_SUCCESS;
					vk_props = (VkLayerProperties *)realloc(vk_props, instance_layer_count * sizeof(VkLayerProperties));
					res = vkEnumerateInstanceLayerProperties(&instance_layer_count, vk_props);
				} while (res == VK_INCOMPLETE);

				for (uint32_t i = 0; i < instance_layer_count; i++) {
					layer_properties layer_props;
					layer_props.properties = vk_props[i];
					res = this->setGlobalExtensionProperties(layer_props);
					if (res) return res;
					info.instance_layer_properties.push_back(layer_props);
				}
				free(vk_props);
				assert(res == VK_SUCCESS);

				return res;
			}

			VkResult setGlobalExtensionProperties(layer_properties &layer_props)
			{
				VkExtensionProperties *instance_extensions;
				uint32_t instance_extension_count;
				VkResult res;
				char *layer_name = NULL;
				layer_name = layer_props.properties.layerName;

				do {
					res = vkEnumerateInstanceExtensionProperties(layer_name, &instance_extension_count, NULL);
					if (res) return res;
					if (instance_extension_count == 0) return VK_SUCCESS;
					layer_props.extensions.resize(instance_extension_count);
					instance_extensions = layer_props.extensions.data();
					res = vkEnumerateInstanceExtensionProperties(layer_name, &instance_extension_count, instance_extensions);
					
				} while (res == VK_INCOMPLETE);

				return res;
			}

			VkApplicationInfo setApplicationInfo()
			{
				VkApplicationInfo app_info = {};
				app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
				app_info.pNext = NULL;
				app_info.pApplicationName = APP_NAME;
				app_info.applicationVersion = 1;
				app_info.pEngineName = APP_NAME;
				app_info.engineVersion = 1;
				app_info.apiVersion = VK_API_VERSION_1_0;
				return app_info;
			}
			VkResult setInstanceInfo(VkApplicationInfo app_info)
			{
				info.instance_extension_names.push_back("VK_KHR_xcb_surface");
				VkInstanceCreateInfo inst_info = {};
				inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
				inst_info.pNext = NULL;
				inst_info.flags = 0;
				inst_info.pApplicationInfo = &app_info;
				inst_info.enabledLayerCount = (uint32_t)info.instance_layer_names.size();
				inst_info.ppEnabledLayerNames = info.instance_layer_names.size() ? info.instance_layer_names.data() : NULL;
				inst_info.enabledExtensionCount = (uint32_t) info.instance_extension_names.size();
				inst_info.ppEnabledExtensionNames = info.instance_extension_names.data();

				VkResult res = vkCreateInstance(&inst_info, NULL, &info.inst);
				assert(res == VK_SUCCESS);

				return res;
			}

			void enumerateDevice(uint32_t gpu_count)
			{
				uint32_t const req_count = gpu_count;
				VkResult res = vkEnumeratePhysicalDevices(this->info.inst, &gpu_count, NULL);
				assert(gpu_count);
				this->info.gpus.resize(gpu_count);

				res = vkEnumeratePhysicalDevices(this->info.inst, &gpu_count, this->info.gpus.data());
				assert(!res && gpu_count >= req_count);

				vkGetPhysicalDeviceQueueFamilyProperties(this->info.gpus[0], &this->info.queue_family_count, NULL);
				assert(this->info.queue_family_count >= 1);

				this->info.queue_props.resize(this->info.queue_family_count);
				vkGetPhysicalDeviceQueueFamilyProperties(this->info.gpus[0], &this->info.queue_family_count, this->info.queue_props.data());
				assert(this->info.queue_family_count >= 1);

				/* This is as good a place as any to do this */
				vkGetPhysicalDeviceMemoryProperties(this->info.gpus[0], &this->info.memory_properties);
				vkGetPhysicalDeviceProperties(this->info.gpus[0], &this->info.gpu_props);
			}


			void createDevice()
			{
				VkDeviceQueueCreateInfo queue_info = {};

				vkGetPhysicalDeviceQueueFamilyProperties(this->info.gpus[0], &this->info.queue_family_count, NULL);
				assert(this->info.queue_family_count >= 1);

				this->info.queue_props.resize(this->info.queue_family_count);
				vkGetPhysicalDeviceQueueFamilyProperties(this->info.gpus[0], &this->info.queue_family_count, this->info.queue_props.data());
				assert(this->info.queue_family_count >= 1);

				bool found = false;
				for (unsigned int i = 0; i < this->info.queue_family_count; i++) {
					if (this->info.queue_props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
						queue_info.queueFamilyIndex = i;
						found = true;
						break;
					}
				}
				assert(found);
				assert(this->info.queue_family_count >= 1);

				float queue_priorities[1] = {0.0};
				queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queue_info.pNext = NULL;
				queue_info.queueCount = 1;
				queue_info.pQueuePriorities = queue_priorities;

				VkDeviceCreateInfo device_info = {};
				device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
				device_info.pNext = NULL;
				device_info.queueCreateInfoCount = 1;
				device_info.pQueueCreateInfos = &queue_info;
				device_info.enabledExtensionCount = (uint32_t)this->info.device_extension_names.size();
				device_info.ppEnabledExtensionNames = device_info.enabledExtensionCount ? this->info.device_extension_names.data() : NULL;
				device_info.enabledLayerCount = 0;
				device_info.ppEnabledLayerNames = NULL;
				device_info.pEnabledFeatures = NULL;

				VkResult res = vkCreateDevice(this->info.gpus[0], &device_info, NULL, &this->info.device);
				assert(res == VK_SUCCESS);
			}
			void createCommandBuffer()
			{
				VkResult U_ASSERT_ONLY res;

				VkCommandPoolCreateInfo cmd_pool_info = {};
				cmd_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
				cmd_pool_info.pNext = NULL;
				cmd_pool_info.queueFamilyIndex = this->info.graphics_queue_family_index;
				cmd_pool_info.flags = 0;

				res = vkCreateCommandPool(this->info.device, &cmd_pool_info, NULL, &this->info.cmd_pool);
				assert(res == VK_SUCCESS);

				/* Create the command buffer from the command pool */
				VkCommandBufferAllocateInfo cmd = {};
				cmd.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				cmd.pNext = NULL;
				cmd.commandPool = this->info.cmd_pool;
				cmd.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
				cmd.commandBufferCount = 1;

				res = vkAllocateCommandBuffers(this->info.device, &cmd, &this->info.cmd);
				assert(res == VK_SUCCESS);
			}
		public:
			void destroyCommandBuffers()
			{
				VkCommandBuffer cmd_bufs[1] = {this->info.cmd};
				vkFreeCommandBuffers(this->info.device, this->info.cmd_pool, 1, cmd_bufs);
				vkDestroyCommandPool(this->info.device, this->info.cmd_pool, NULL);
			}

			void destroyInstance()
			{
				vkDestroyInstance(this->info.inst, NULL);
			}

			void destroyDevice()
			{
				vkDestroyDevice(this->device, NULL);
			}

			void logExtensions()
			{
				VkResult res;
				std::string prefix = "[EXT] ";
				do {
					uint32_t count;
					res = vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr); //get number of extensions
					std::vector<VkExtensionProperties> extensions(count);
					res = vkEnumerateInstanceExtensionProperties(nullptr, &count, extensions.data()); //populate buffer

					for (auto & extension : extensions) {
						this->log(prefix + extension.extensionName);
					}
					
				} while (res == VK_INCOMPLETE);
			}
		};
	}
}

#endif //OBSIDIAN2D_RENDERER_WINDOW_
