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
		private:
			bool memory_type_from_properties(struct VulkanInfo &info, uint32_t typeBits, VkFlags requirements_mask, uint32_t *typeIndex) {
				// Search memtypes to find first index with those properties
				for (uint32_t i = 0; i < info.memory_properties.memoryTypeCount; i++) {
					if ((typeBits & 1) == 1) {
						// Type is available, does it match user properties?
						if ((info.memory_properties.memoryTypes[i].propertyFlags & requirements_mask) == requirements_mask) {
							*typeIndex = i;
							return true;
						}
					}
					typeBits >>= 1;
				}
				// No memory types matched, return failure
				return false;
			}
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
			void pushBackExtensions(){
				this->info.instance_extension_names.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
#ifdef _WIN32
				this->info.instance_extension_names.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif __ANDROID__
				this->info.instance_extension_names.push_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);
#else
				this->info.instance_extension_names.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#endif
				info.device_extension_names.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
			}
			VkResult setInstanceInfo(VkApplicationInfo app_info)
			{
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
			void executeBeginCommandBuffer()
			{
				/* DEPENDS on init_command_buffer() */
				VkResult U_ASSERT_ONLY res;

				VkCommandBufferBeginInfo cmd_buf_info = {};
				cmd_buf_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				cmd_buf_info.pNext = NULL;
				cmd_buf_info.flags = 0;
				cmd_buf_info.pInheritanceInfo = NULL;

				res = vkBeginCommandBuffer(this->info.cmd, &cmd_buf_info);
				assert(res == VK_SUCCESS);
			}
			void initDeviceQueue()
			{
				/* DEPENDS on init_swapchain_extension() */
				vkGetDeviceQueue(this->info.device, this->info.graphics_queue_family_index, 0, &this->info.graphics_queue);
				if (this->info.graphics_queue_family_index == this->info.present_queue_family_index) {
					this->info.present_queue = this->info.graphics_queue;
				} else {
					vkGetDeviceQueue(this->info.device, this->info.present_queue_family_index, 0, &this->info.present_queue);
				}
			}
			void initSwapChain(VkImageUsageFlags usageFlags = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT)
			{
				/* DEPENDS on info.cmd and info.queue initialized */

				VkResult U_ASSERT_ONLY res;
				VkSurfaceCapabilitiesKHR surfCapabilities;

				res = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(this->info.gpus[0], this->info.surface, &surfCapabilities);
				assert(res == VK_SUCCESS);

				uint32_t presentModeCount;
				res = vkGetPhysicalDeviceSurfacePresentModesKHR(this->info.gpus[0], this->info.surface, &presentModeCount, NULL);
				assert(res == VK_SUCCESS);
				VkPresentModeKHR *presentModes = (VkPresentModeKHR *)malloc(presentModeCount * sizeof(VkPresentModeKHR));
				assert(presentModes);
				res = vkGetPhysicalDeviceSurfacePresentModesKHR(this->info.gpus[0], this->info.surface, &presentModeCount, presentModes);
				assert(res == VK_SUCCESS);

				VkExtent2D swapchainExtent;
				// width and height are either both 0xFFFFFFFF, or both not 0xFFFFFFFF.
				if (surfCapabilities.currentExtent.width == 0xFFFFFFFF) {
					// If the surface size is undefined, the size is set to
					// the size of the images requested.
					swapchainExtent.width =  (uint32_t)this->info.width;
					swapchainExtent.height = (uint32_t)this->info.height;
					if (swapchainExtent.width < surfCapabilities.minImageExtent.width) {
						swapchainExtent.width = surfCapabilities.minImageExtent.width;
					} else if (swapchainExtent.width > surfCapabilities.maxImageExtent.width) {
						swapchainExtent.width = surfCapabilities.maxImageExtent.width;
					}

					if (swapchainExtent.height < surfCapabilities.minImageExtent.height) {
						swapchainExtent.height = surfCapabilities.minImageExtent.height;
					} else if (swapchainExtent.height > surfCapabilities.maxImageExtent.height) {
						swapchainExtent.height = surfCapabilities.maxImageExtent.height;
					}
				} else {
					// If the surface size is defined, the swap chain size must match
					swapchainExtent = surfCapabilities.currentExtent;
				}

				// The FIFO present mode is guaranteed by the spec to be supported
				// Also note that current Android driver only supports FIFO
				VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;

				// Determine the number of VkImage's to use in the swap chain.
				// We need to acquire only 1 presentable image at at time.
				// Asking for minImageCount images ensures that we can acquire
				// 1 presentable image as long as we present it before attempting
				// to acquire another.
				uint32_t desiredNumberOfSwapChainImages = surfCapabilities.minImageCount;

				VkSurfaceTransformFlagBitsKHR preTransform;
				if (surfCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
					preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
				} else {
					preTransform = surfCapabilities.currentTransform;
				}

				// Find a supported composite alpha mode - one of these is guaranteed to be set
				VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
				VkCompositeAlphaFlagBitsKHR compositeAlphaFlags[4] = {
						VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
						VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
						VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
						VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
				};
				for (uint32_t i = 0; i < sizeof(compositeAlphaFlags); i++) {
					if (surfCapabilities.supportedCompositeAlpha & compositeAlphaFlags[i]) {
						compositeAlpha = compositeAlphaFlags[i];
						break;
					}
				}

				VkSwapchainCreateInfoKHR swapchain_ci = {};
				swapchain_ci.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
				swapchain_ci.pNext = NULL;
				swapchain_ci.surface = info.surface;
				swapchain_ci.minImageCount = desiredNumberOfSwapChainImages;
				swapchain_ci.imageFormat = info.format;
				swapchain_ci.imageExtent.width = swapchainExtent.width;
				swapchain_ci.imageExtent.height = swapchainExtent.height;
				swapchain_ci.preTransform = preTransform;
				swapchain_ci.compositeAlpha = compositeAlpha;
				swapchain_ci.imageArrayLayers = 1;
				swapchain_ci.presentMode = swapchainPresentMode;
				swapchain_ci.oldSwapchain = VK_NULL_HANDLE;
#ifndef __ANDROID__
				swapchain_ci.clipped = (VkBool32)true;
#else
				swapchain_ci.clipped = false;
#endif
				swapchain_ci.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
				swapchain_ci.imageUsage = usageFlags;
				swapchain_ci.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
				swapchain_ci.queueFamilyIndexCount = 0;
				swapchain_ci.pQueueFamilyIndices = NULL;
				uint32_t queueFamilyIndices[2] = {(uint32_t)info.graphics_queue_family_index, (uint32_t)info.present_queue_family_index};
				if (info.graphics_queue_family_index != info.present_queue_family_index) {
					// If the graphics and present queues are from different queue families,
					// we either have to explicitly transfer ownership of images between the
					// queues, or we have to create the swapchain with imageSharingMode
					// as VK_SHARING_MODE_CONCURRENT
					swapchain_ci.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
					swapchain_ci.queueFamilyIndexCount = 2;
					swapchain_ci.pQueueFamilyIndices = queueFamilyIndices;
				}

				res = vkCreateSwapchainKHR(info.device, &swapchain_ci, NULL, &info.swap_chain);
				assert(res == VK_SUCCESS);

				res = vkGetSwapchainImagesKHR(info.device, info.swap_chain, &info.swapchainImageCount, NULL);
				assert(res == VK_SUCCESS);

				VkImage *swapchainImages = (VkImage *)malloc(info.swapchainImageCount * sizeof(VkImage));
				assert(swapchainImages);
				res = vkGetSwapchainImagesKHR(info.device, info.swap_chain, &info.swapchainImageCount, swapchainImages);
				assert(res == VK_SUCCESS);

				for (uint32_t i = 0; i < info.swapchainImageCount; i++) {
					swap_chain_buffer sc_buffer;

					VkImageViewCreateInfo color_image_view = {};
					color_image_view.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
					color_image_view.pNext = NULL;
					color_image_view.format = info.format;
					color_image_view.components.r = VK_COMPONENT_SWIZZLE_R;
					color_image_view.components.g = VK_COMPONENT_SWIZZLE_G;
					color_image_view.components.b = VK_COMPONENT_SWIZZLE_B;
					color_image_view.components.a = VK_COMPONENT_SWIZZLE_A;
					color_image_view.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					color_image_view.subresourceRange.baseMipLevel = 0;
					color_image_view.subresourceRange.levelCount = 1;
					color_image_view.subresourceRange.baseArrayLayer = 0;
					color_image_view.subresourceRange.layerCount = 1;
					color_image_view.viewType = VK_IMAGE_VIEW_TYPE_2D;
					color_image_view.flags = 0;

					sc_buffer.image = swapchainImages[i];

					color_image_view.image = sc_buffer.image;

					res = vkCreateImageView(info.device, &color_image_view, NULL, &sc_buffer.view);
					info.buffers.push_back(sc_buffer);
					assert(res == VK_SUCCESS);
				}
				free(swapchainImages);
				info.current_buffer = 0;

				if (NULL != presentModes) {
					free(presentModes);
				}
			}
			void initDepthBuffer()
			{
				VkResult U_ASSERT_ONLY res;
				bool U_ASSERT_ONLY pass;
				VkImageCreateInfo image_info = {};

				/* allow custom depth formats */
				if (this->info.depth.format == VK_FORMAT_UNDEFINED) this->info.depth.format = VK_FORMAT_D16_UNORM;

#ifdef __ANDROID__
				// Depth format needs to be VK_FORMAT_D24_UNORM_S8_UINT on Android.
    const VkFormat depth_format = VK_FORMAT_D24_UNORM_S8_UINT;
#else
				const VkFormat depth_format = this->info.depth.format;
#endif
				VkFormatProperties props;
				vkGetPhysicalDeviceFormatProperties(this->info.gpus[0], depth_format, &props);
				if (props.linearTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
					image_info.tiling = VK_IMAGE_TILING_LINEAR;
				} else if (props.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
					image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
				} else {
					/* Try other depth formats? */
					std::cout << "depth_format " << depth_format << " Unsupported.\n";
					exit(-1);
				}

				image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
				image_info.pNext = NULL;
				image_info.imageType = VK_IMAGE_TYPE_2D;
				image_info.format = depth_format;
				image_info.extent.width = (uint32_t)this->info.width;
				image_info.extent.height = (uint32_t)this->info.height;
				image_info.extent.depth = 1;
				image_info.mipLevels = 1;
				image_info.arrayLayers = 1;
				image_info.samples = VK_SAMPLE_COUNT_1_BIT;
				image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				image_info.queueFamilyIndexCount = 0;
				image_info.pQueueFamilyIndices = NULL;
				image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
				image_info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
				image_info.flags = 0;

				VkMemoryAllocateInfo mem_alloc = {};
				mem_alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				mem_alloc.pNext = NULL;
				mem_alloc.allocationSize = 0;
				mem_alloc.memoryTypeIndex = 0;

				VkImageViewCreateInfo view_info = {};
				view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				view_info.pNext = NULL;
				view_info.image = VK_NULL_HANDLE;
				view_info.format = depth_format;
				view_info.components.r = VK_COMPONENT_SWIZZLE_R;
				view_info.components.g = VK_COMPONENT_SWIZZLE_G;
				view_info.components.b = VK_COMPONENT_SWIZZLE_B;
				view_info.components.a = VK_COMPONENT_SWIZZLE_A;
				view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
				view_info.subresourceRange.baseMipLevel = 0;
				view_info.subresourceRange.levelCount = 1;
				view_info.subresourceRange.baseArrayLayer = 0;
				view_info.subresourceRange.layerCount = 1;
				view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
				view_info.flags = 0;

				if (depth_format == VK_FORMAT_D16_UNORM_S8_UINT || depth_format == VK_FORMAT_D24_UNORM_S8_UINT ||
					depth_format == VK_FORMAT_D32_SFLOAT_S8_UINT) {
					view_info.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
				}

				VkMemoryRequirements mem_reqs;

				/* Create image */
				res = vkCreateImage(this->info.device, &image_info, NULL, &this->info.depth.image);
				assert(res == VK_SUCCESS);

				vkGetImageMemoryRequirements(this->info.device, this->info.depth.image, &mem_reqs);

				mem_alloc.allocationSize = mem_reqs.size;
				/* Use the memory properties to determine the type of memory required */
				pass = this->memory_type_from_properties(this->info, mem_reqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &mem_alloc.memoryTypeIndex);
				assert(pass);

				/* Allocate memory */
				res = vkAllocateMemory(this->info.device, &mem_alloc, NULL, &this->info.depth.mem);
				assert(res == VK_SUCCESS);

				/* Bind memory */
				res = vkBindImageMemory(this->info.device, this->info.depth.image, this->info.depth.mem, 0);
				assert(res == VK_SUCCESS);

				/* Create image view */
				view_info.image = this->info.depth.image;
				res = vkCreateImageView(this->info.device, &view_info, NULL, &this->info.depth.view);
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
