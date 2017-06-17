#ifndef OBSIDIAN2D_RENDERER_WINDOW_
#define OBSIDIAN2D_RENDERER_WINDOW_

#include <malloc.h>
#include <assert.h>

#include <string>
#include <cstring>
#include <zconf.h>
#include <iosfwd>

#include "Camera.h"

struct Vertex {
    float posX, posY, posZ, posW;  // Position data
    float r, g, b, a;              // Color
};

struct VertexUV {
    float posX, posY, posZ, posW;  // Position data
    float u, v;                    // texture u,v
};

#define XYZ1(_x_, _y_, _z_) (_x_), (_y_), (_z_), 1.f
#define UV(_u_, _v_) (_u_), (_v_)

static const Vertex g_vbData[] = {
    {XYZ1(-1, -1, -1), XYZ1(0.f, 0.f, 0.f)}, {XYZ1(1, -1, -1), XYZ1(1.f, 0.f, 0.f)}, {XYZ1(-1, 1, -1), XYZ1(0.f, 1.f, 0.f)},
    {XYZ1(-1, 1, -1), XYZ1(0.f, 1.f, 0.f)},  {XYZ1(1, -1, -1), XYZ1(1.f, 0.f, 0.f)}, {XYZ1(1, 1, -1), XYZ1(1.f, 1.f, 0.f)},

    {XYZ1(-1, -1, 1), XYZ1(0.f, 0.f, 1.f)},  {XYZ1(-1, 1, 1), XYZ1(0.f, 1.f, 1.f)},  {XYZ1(1, -1, 1), XYZ1(1.f, 0.f, 1.f)},
    {XYZ1(1, -1, 1), XYZ1(1.f, 0.f, 1.f)},   {XYZ1(-1, 1, 1), XYZ1(0.f, 1.f, 1.f)},  {XYZ1(1, 1, 1), XYZ1(1.f, 1.f, 1.f)},

    {XYZ1(1, 1, 1), XYZ1(1.f, 1.f, 1.f)},    {XYZ1(1, 1, -1), XYZ1(1.f, 1.f, 0.f)},  {XYZ1(1, -1, 1), XYZ1(1.f, 0.f, 1.f)},
    {XYZ1(1, -1, 1), XYZ1(1.f, 0.f, 1.f)},   {XYZ1(1, 1, -1), XYZ1(1.f, 1.f, 0.f)},  {XYZ1(1, -1, -1), XYZ1(1.f, 0.f, 0.f)},

    {XYZ1(-1, 1, 1), XYZ1(0.f, 1.f, 1.f)},   {XYZ1(-1, -1, 1), XYZ1(0.f, 0.f, 1.f)}, {XYZ1(-1, 1, -1), XYZ1(0.f, 1.f, 0.f)},
    {XYZ1(-1, 1, -1), XYZ1(0.f, 1.f, 0.f)},  {XYZ1(-1, -1, 1), XYZ1(0.f, 0.f, 1.f)}, {XYZ1(-1, -1, -1), XYZ1(0.f, 0.f, 0.f)},

    {XYZ1(1, 1, 1), XYZ1(1.f, 1.f, 1.f)},    {XYZ1(-1, 1, 1), XYZ1(0.f, 1.f, 1.f)},  {XYZ1(1, 1, -1), XYZ1(1.f, 1.f, 0.f)},
    {XYZ1(1, 1, -1), XYZ1(1.f, 1.f, 0.f)},   {XYZ1(-1, 1, 1), XYZ1(0.f, 1.f, 1.f)},  {XYZ1(-1, 1, -1), XYZ1(0.f, 1.f, 0.f)},

    {XYZ1(1, -1, 1), XYZ1(1.f, 0.f, 1.f)},   {XYZ1(1, -1, -1), XYZ1(1.f, 0.f, 0.f)}, {XYZ1(-1, -1, 1), XYZ1(0.f, 0.f, 1.f)},
    {XYZ1(-1, -1, 1), XYZ1(0.f, 0.f, 1.f)},  {XYZ1(1, -1, -1), XYZ1(1.f, 0.f, 0.f)}, {XYZ1(-1, -1, -1), XYZ1(0.f, 0.f, 0.f)},
};

static const Vertex g_vb_solid_face_colors_Data[] = {
    // red face
    {XYZ1(-1, -1, 1), XYZ1(1.f, 0.f, 0.f)},
    {XYZ1(-1, 1, 1), XYZ1(1.f, 0.f, 0.f)},
    {XYZ1(1, -1, 1), XYZ1(1.f, 0.f, 0.f)},
    {XYZ1(1, -1, 1), XYZ1(1.f, 0.f, 0.f)},
    {XYZ1(-1, 1, 1), XYZ1(1.f, 0.f, 0.f)},
    {XYZ1(1, 1, 1), XYZ1(1.f, 0.f, 0.f)},
    // green face
    {XYZ1(-1, -1, -1), XYZ1(0.f, 1.f, 0.f)},
    {XYZ1(1, -1, -1), XYZ1(0.f, 1.f, 0.f)},
    {XYZ1(-1, 1, -1), XYZ1(0.f, 1.f, 0.f)},
    {XYZ1(-1, 1, -1), XYZ1(0.f, 1.f, 0.f)},
    {XYZ1(1, -1, -1), XYZ1(0.f, 1.f, 0.f)},
    {XYZ1(1, 1, -1), XYZ1(0.f, 1.f, 0.f)},
    // blue face
    {XYZ1(-1, 1, 1), XYZ1(0.f, 0.f, 1.f)},
    {XYZ1(-1, -1, 1), XYZ1(0.f, 0.f, 1.f)},
    {XYZ1(-1, 1, -1), XYZ1(0.f, 0.f, 1.f)},
    {XYZ1(-1, 1, -1), XYZ1(0.f, 0.f, 1.f)},
    {XYZ1(-1, -1, 1), XYZ1(0.f, 0.f, 1.f)},
    {XYZ1(-1, -1, -1), XYZ1(0.f, 0.f, 1.f)},
    // yellow face
    {XYZ1(1, 1, 1), XYZ1(1.f, 1.f, 0.f)},
    {XYZ1(1, 1, -1), XYZ1(1.f, 1.f, 0.f)},
    {XYZ1(1, -1, 1), XYZ1(1.f, 1.f, 0.f)},
    {XYZ1(1, -1, 1), XYZ1(1.f, 1.f, 0.f)},
    {XYZ1(1, 1, -1), XYZ1(1.f, 1.f, 0.f)},
    {XYZ1(1, -1, -1), XYZ1(1.f, 1.f, 0.f)},
    // magenta face
    {XYZ1(1, 1, 1), XYZ1(1.f, 0.f, 1.f)},
    {XYZ1(-1, 1, 1), XYZ1(1.f, 0.f, 1.f)},
    {XYZ1(1, 1, -1), XYZ1(1.f, 0.f, 1.f)},
    {XYZ1(1, 1, -1), XYZ1(1.f, 0.f, 1.f)},
    {XYZ1(-1, 1, 1), XYZ1(1.f, 0.f, 1.f)},
    {XYZ1(-1, 1, -1), XYZ1(1.f, 0.f, 1.f)},
    // cyan face
    {XYZ1(1, -1, 1), XYZ1(0.f, 1.f, 1.f)},
    {XYZ1(1, -1, -1), XYZ1(0.f, 1.f, 1.f)},
    {XYZ1(-1, -1, 1), XYZ1(0.f, 1.f, 1.f)},
    {XYZ1(-1, -1, 1), XYZ1(0.f, 1.f, 1.f)},
    {XYZ1(1, -1, -1), XYZ1(0.f, 1.f, 1.f)},
    {XYZ1(-1, -1, -1), XYZ1(0.f, 1.f, 1.f)},
};

static const VertexUV g_vb_texture_Data[] = {
    // left face
    {XYZ1(-1, -1, -1), UV(1.f, 0.f)},  // lft-top-front
    {XYZ1(-1, 1, 1), UV(0.f, 1.f)},    // lft-btm-back
    {XYZ1(-1, -1, 1), UV(0.f, 0.f)},   // lft-top-back
    {XYZ1(-1, 1, 1), UV(0.f, 1.f)},    // lft-btm-back
    {XYZ1(-1, -1, -1), UV(1.f, 0.f)},  // lft-top-front
    {XYZ1(-1, 1, -1), UV(1.f, 1.f)},   // lft-btm-front
    // front face
    {XYZ1(-1, -1, -1), UV(0.f, 0.f)},  // lft-top-front
    {XYZ1(1, -1, -1), UV(1.f, 0.f)},   // rgt-top-front
    {XYZ1(1, 1, -1), UV(1.f, 1.f)},    // rgt-btm-front
    {XYZ1(-1, -1, -1), UV(0.f, 0.f)},  // lft-top-front
    {XYZ1(1, 1, -1), UV(1.f, 1.f)},    // rgt-btm-front
    {XYZ1(-1, 1, -1), UV(0.f, 1.f)},   // lft-btm-front
    // top face
    {XYZ1(-1, -1, -1), UV(0.f, 1.f)},  // lft-top-front
    {XYZ1(1, -1, 1), UV(1.f, 0.f)},    // rgt-top-back
    {XYZ1(1, -1, -1), UV(1.f, 1.f)},   // rgt-top-front
    {XYZ1(-1, -1, -1), UV(0.f, 1.f)},  // lft-top-front
    {XYZ1(-1, -1, 1), UV(0.f, 0.f)},   // lft-top-back
    {XYZ1(1, -1, 1), UV(1.f, 0.f)},    // rgt-top-back
    // bottom face
    {XYZ1(-1, 1, -1), UV(0.f, 0.f)},  // lft-btm-front
    {XYZ1(1, 1, 1), UV(1.f, 1.f)},    // rgt-btm-back
    {XYZ1(-1, 1, 1), UV(0.f, 1.f)},   // lft-btm-back
    {XYZ1(-1, 1, -1), UV(0.f, 0.f)},  // lft-btm-front
    {XYZ1(1, 1, -1), UV(1.f, 0.f)},   // rgt-btm-front
    {XYZ1(1, 1, 1), UV(1.f, 1.f)},    // rgt-btm-back
    // right face
    {XYZ1(1, 1, -1), UV(0.f, 1.f)},   // rgt-btm-front
    {XYZ1(1, -1, 1), UV(1.f, 0.f)},   // rgt-top-back
    {XYZ1(1, 1, 1), UV(1.f, 1.f)},    // rgt-btm-back
    {XYZ1(1, -1, 1), UV(1.f, 0.f)},   // rgt-top-back
    {XYZ1(1, 1, -1), UV(0.f, 1.f)},   // rgt-btm-front
    {XYZ1(1, -1, -1), UV(0.f, 0.f)},  // rgt-top-front
    // back face
    {XYZ1(-1, 1, 1), UV(1.f, 1.f)},   // lft-btm-back
    {XYZ1(1, 1, 1), UV(0.f, 1.f)},    // rgt-btm-back
    {XYZ1(-1, -1, 1), UV(1.f, 0.f)},  // lft-top-back
    {XYZ1(-1, -1, 1), UV(1.f, 0.f)},  // lft-top-back
    {XYZ1(1, 1, 1), UV(0.f, 1.f)},    // rgt-btm-back
    {XYZ1(1, -1, 1), UV(0.f, 0.f)},   // rgt-top-back
};


#define APP_NAME "Obsidian2D"

namespace Obsidian2D
{
	namespace Renderer
	{
		class Window : public Camera
		{
		protected:
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

			void pushBackExtensions()
			{
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

				res = vkCreateSwapchainKHR(this->info.device, &swapchain_ci, NULL, &this->info.swap_chain);
				assert(res == VK_SUCCESS);

				res = vkGetSwapchainImagesKHR(this->info.device, this->info.swap_chain, &this->info.swapchainImageCount, NULL);
				assert(res == VK_SUCCESS);

				VkImage *swapchainImages = (VkImage *)malloc(this->info.swapchainImageCount * sizeof(VkImage));
				assert(swapchainImages);
				res = vkGetSwapchainImagesKHR(this->info.device, this->info.swap_chain, &this->info.swapchainImageCount, swapchainImages);
				assert(res == VK_SUCCESS);

				for (uint32_t i = 0; i < this->info.swapchainImageCount; i++) {
					swap_chain_buffer sc_buffer;

					VkImageViewCreateInfo color_image_view = {};
					color_image_view.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
					color_image_view.pNext = NULL;
					color_image_view.format = this->info.format;
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

					res = vkCreateImageView(this->info.device, &color_image_view, NULL, &sc_buffer.view);
					this->info.buffers.push_back(sc_buffer);
					assert(res == VK_SUCCESS);
				}
				free(swapchainImages);
				this->info.current_buffer = 0;

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

			void initUniformBuffer()
			{
				VkResult U_ASSERT_ONLY res;
				bool U_ASSERT_ONLY pass;

				this->initCamera(this->info.width, this->info.height);

				uint8_t *pData;
				VkBufferCreateInfo buf_info = {};
				buf_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
				buf_info.pNext = NULL;
				buf_info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
				buf_info.size = sizeof(this->info.MVP);
				buf_info.queueFamilyIndexCount = 0;
				buf_info.pQueueFamilyIndices = NULL;
				buf_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
				buf_info.flags = 0;
				res = vkCreateBuffer(info.device, &buf_info, NULL, &this->info.uniform_data.buf);
				assert(res == VK_SUCCESS);

				VkMemoryRequirements mem_reqs;
				vkGetBufferMemoryRequirements(this->info.device, this->info.uniform_data.buf, &mem_reqs);

				VkMemoryAllocateInfo alloc_info = {};
				alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				alloc_info.pNext = NULL;
				alloc_info.memoryTypeIndex = 0;

				alloc_info.allocationSize = mem_reqs.size;
				pass = memory_type_from_properties(this->info, mem_reqs.memoryTypeBits,
												   VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
												   &alloc_info.memoryTypeIndex);
				assert(pass && "No mappable, coherent memory");

				res = vkAllocateMemory(this->info.device, &alloc_info, NULL, &(this->info.uniform_data.mem));
				assert(res == VK_SUCCESS);

				res = vkMapMemory(this->info.device, this->info.uniform_data.mem, 0, mem_reqs.size, 0, (void **)&pData);
				assert(res == VK_SUCCESS);

				memcpy(pData, &this->info.MVP, sizeof(this->info.MVP));
				this->setCameraBufferAddress(pData);

				vkUnmapMemory(this->info.device, this->info.uniform_data.mem);

				res = vkBindBufferMemory(this->info.device, this->info.uniform_data.buf, this->info.uniform_data.mem, 0);
				assert(res == VK_SUCCESS);

				this->info.uniform_data.buffer_info.buffer = this->info.uniform_data.buf;
				this->info.uniform_data.buffer_info.offset = 0;
				this->info.uniform_data.buffer_info.range = sizeof(this->info.MVP);
			}

			void initDescriptorAndPipelineLayouts(bool use_texture)
			{
				VkDescriptorSetLayoutBinding layout_bindings[2];
				layout_bindings[0].binding = 0;
				layout_bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				layout_bindings[0].descriptorCount = 1;
				layout_bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
				layout_bindings[0].pImmutableSamplers = NULL;

				if (use_texture) {
					layout_bindings[1].binding = 1;
					layout_bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					layout_bindings[1].descriptorCount = 1;
					layout_bindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
					layout_bindings[1].pImmutableSamplers = NULL;
				}

				/* Next take layout bindings and use them to create a descriptor set layout
				 */
				VkDescriptorSetLayoutCreateInfo descriptor_layout = {};
				descriptor_layout.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
				descriptor_layout.pNext = NULL;
				descriptor_layout.bindingCount = use_texture ? 2 : 1;
				descriptor_layout.pBindings = layout_bindings;

				VkResult U_ASSERT_ONLY res;

				this->info.desc_layout.resize(1);
				res = vkCreateDescriptorSetLayout(this->info.device, &descriptor_layout, NULL, this->info.desc_layout.data());
				assert(res == VK_SUCCESS);

				/* Now use the descriptor layout to create a pipeline layout */
				VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo = {};
				pPipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
				pPipelineLayoutCreateInfo.pNext = NULL;
				pPipelineLayoutCreateInfo.pushConstantRangeCount = 0;
				pPipelineLayoutCreateInfo.pPushConstantRanges = NULL;
				pPipelineLayoutCreateInfo.setLayoutCount = 1;
				pPipelineLayoutCreateInfo.pSetLayouts = this->info.desc_layout.data();

				res = vkCreatePipelineLayout(info.device, &pPipelineLayoutCreateInfo, NULL, &this->info.pipeline_layout);
				assert(res == VK_SUCCESS);
			}

			void initRenderpass(bool include_depth, bool clear = true, VkImageLayout finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
			{
				/* DEPENDS on init_swap_chain() and init_depth_buffer() */

				VkResult U_ASSERT_ONLY res;
				/* Need attachments for render target and depth buffer */
				VkAttachmentDescription attachments[2];
				attachments[0].format = this->info.format;
				attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
				attachments[0].loadOp = clear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				attachments[0].finalLayout = finalLayout;
				attachments[0].flags = 0;

				if (include_depth) {
					attachments[1].format = this->info.depth.format;
					attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
					attachments[1].loadOp = clear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_DONT_CARE;
					attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
					attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
					attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
					attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
					attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
					attachments[1].flags = 0;
				}

				VkAttachmentReference color_reference = {};
				color_reference.attachment = 0;
				color_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

				VkAttachmentReference depth_reference = {};
				depth_reference.attachment = 1;
				depth_reference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

				VkSubpassDescription subpass = {};
				subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
				subpass.flags = 0;
				subpass.inputAttachmentCount = 0;
				subpass.pInputAttachments = NULL;
				subpass.colorAttachmentCount = 1;
				subpass.pColorAttachments = &color_reference;
				subpass.pResolveAttachments = NULL;
				subpass.pDepthStencilAttachment = include_depth ? &depth_reference : NULL;
				subpass.preserveAttachmentCount = 0;
				subpass.pPreserveAttachments = NULL;

				VkRenderPassCreateInfo rp_info = {};
				rp_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
				rp_info.pNext = NULL;
				rp_info.attachmentCount = include_depth ? 2 : 1;
				rp_info.pAttachments = attachments;
				rp_info.subpassCount = 1;
				rp_info.pSubpasses = &subpass;
				rp_info.dependencyCount = 0;
				rp_info.pDependencies = NULL;

				res = vkCreateRenderPass(this->info.device, &rp_info, NULL, &this->info.render_pass);
				assert(res == VK_SUCCESS);
			}

			void initShaders()
			{
				VkResult U_ASSERT_ONLY res;
				bool U_ASSERT_ONLY retVal;

				// If no shaders were submitted, just return
				if (!(this->initialVertShaderText || this->initialFragShaderText)) return;

				init_glslang();
				VkShaderModuleCreateInfo moduleCreateInfo;

				if (this->initialVertShaderText) {
					std::vector<unsigned int> vtx_spv;
					this->info.shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
					this->info.shaderStages[0].pNext = NULL;
					this->info.shaderStages[0].pSpecializationInfo = NULL;
					this->info.shaderStages[0].flags = 0;
					this->info.shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
					this->info.shaderStages[0].pName = "main";

					retVal = GLSLtoSPV(VK_SHADER_STAGE_VERTEX_BIT, this->initialVertShaderText, vtx_spv);
					assert(retVal);

					moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
					moduleCreateInfo.pNext = NULL;
					moduleCreateInfo.flags = 0;
					moduleCreateInfo.codeSize = vtx_spv.size() * sizeof(unsigned int);
					moduleCreateInfo.pCode = vtx_spv.data();
					res = vkCreateShaderModule(info.device, &moduleCreateInfo, NULL, &info.shaderStages[0].module);
					assert(res == VK_SUCCESS);
				}

				if (this->initialFragShaderText) {
					std::vector<unsigned int> frag_spv;
					this->info.shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
					this->info.shaderStages[1].pNext = NULL;
					this->info.shaderStages[1].pSpecializationInfo = NULL;
					this->info.shaderStages[1].flags = 0;
					this->info.shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
					this->info.shaderStages[1].pName = "main";

					retVal = GLSLtoSPV(VK_SHADER_STAGE_FRAGMENT_BIT, this->initialFragShaderText, frag_spv);
					assert(retVal);

					moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
					moduleCreateInfo.pNext = NULL;
					moduleCreateInfo.flags = 0;
					moduleCreateInfo.codeSize = frag_spv.size() * sizeof(unsigned int);
					moduleCreateInfo.pCode = frag_spv.data();
					res = vkCreateShaderModule(this->info.device, &moduleCreateInfo, NULL, &this->info.shaderStages[1].module);
					assert(res == VK_SUCCESS);
				}

				finalize_glslang();
			}

			void initFramebuffers(bool include_depth)
			{
				VkResult U_ASSERT_ONLY res;
				VkImageView attachments[2];
				attachments[1] = this->info.depth.view;

				VkFramebufferCreateInfo fb_info = {};
				fb_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
				fb_info.pNext = NULL;
				fb_info.renderPass = this->info.render_pass;
				fb_info.attachmentCount = include_depth ? 2 : 1;
				fb_info.pAttachments = attachments;
				fb_info.width = (uint32_t)this->info.width;
				fb_info.height = (uint32_t)this->info.height;
				fb_info.layers = 1;

				uint32_t i;

				this->info.framebuffers = (VkFramebuffer *)malloc(this->info.swapchainImageCount * sizeof(VkFramebuffer));

				for (i = 0; i < this->info.swapchainImageCount; i++) {
					attachments[0] = this->info.buffers[i].view;
					res = vkCreateFramebuffer(this->info.device, &fb_info, NULL, &this->info.framebuffers[i]);
					assert(res == VK_SUCCESS);
				}
			}

			void initVertexBuffer(const void *vertexData, uint32_t dataSize, uint32_t dataStride, bool use_texture)
			{
				VkResult U_ASSERT_ONLY res;
				bool U_ASSERT_ONLY pass;

				VkBufferCreateInfo buf_info = {};
				buf_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
				buf_info.pNext = NULL;
				buf_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
				buf_info.size = dataSize;
				buf_info.queueFamilyIndexCount = 0;
				buf_info.pQueueFamilyIndices = NULL;
				buf_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
				buf_info.flags = 0;
				res = vkCreateBuffer(this->info.device, &buf_info, NULL, &this->info.vertex_buffer.buf);
				assert(res == VK_SUCCESS);

				VkMemoryRequirements mem_reqs;
				vkGetBufferMemoryRequirements(this->info.device, this->info.vertex_buffer.buf, &mem_reqs);

				VkMemoryAllocateInfo alloc_info = {};
				alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				alloc_info.pNext = NULL;
				alloc_info.memoryTypeIndex = 0;

				alloc_info.allocationSize = mem_reqs.size;
				pass = memory_type_from_properties(this->info, mem_reqs.memoryTypeBits,
												   VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
												   &alloc_info.memoryTypeIndex);
				assert(pass && "No mappable, coherent memory");

				res = vkAllocateMemory(info.device, &alloc_info, NULL, &(this->info.vertex_buffer.mem));
				assert(res == VK_SUCCESS);
				this->info.vertex_buffer.buffer_info.range = mem_reqs.size;
				this->info.vertex_buffer.buffer_info.offset = 0;

				uint8_t *pData;
				res = vkMapMemory(this->info.device, this->info.vertex_buffer.mem, 0, mem_reqs.size, 0, (void **)&pData);
				assert(res == VK_SUCCESS);

				memcpy(pData, vertexData, dataSize);

				vkUnmapMemory(this->info.device, this->info.vertex_buffer.mem);

				res = vkBindBufferMemory(this->info.device, this->info.vertex_buffer.buf, this->info.vertex_buffer.mem, 0);
				assert(res == VK_SUCCESS);

				this->info.vi_binding.binding = 0;
				this->info.vi_binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
				this->info.vi_binding.stride = dataStride;

				this->info.vi_attribs[0].binding = 0;
				this->info.vi_attribs[0].location = 0;
				this->info.vi_attribs[0].format = VK_FORMAT_R32G32B32A32_SFLOAT;
				this->info.vi_attribs[0].offset = 0;
				this->info.vi_attribs[1].binding = 0;
				this->info.vi_attribs[1].location = 1;
				this->info.vi_attribs[1].format = use_texture ? VK_FORMAT_R32G32_SFLOAT : VK_FORMAT_R32G32B32A32_SFLOAT;
				this->info.vi_attribs[1].offset = 16;
			}

			void drawCube(){

				VkResult U_ASSERT_ONLY res;

				VkClearValue clear_values[2];
				clear_values[0].color.float32[0] = 0.2f;
				clear_values[0].color.float32[1] = 0.2f;
				clear_values[0].color.float32[2] = 0.2f;
				clear_values[0].color.float32[3] = 0.2f;
				clear_values[1].depthStencil.depth = 1.0f;
				clear_values[1].depthStencil.stencil = 0;

				VkSemaphore imageAcquiredSemaphore;
				VkSemaphoreCreateInfo imageAcquiredSemaphoreCreateInfo;
				imageAcquiredSemaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
				imageAcquiredSemaphoreCreateInfo.pNext = NULL;
				imageAcquiredSemaphoreCreateInfo.flags = 0;

				res = vkCreateSemaphore(this->info.device, &imageAcquiredSemaphoreCreateInfo, NULL, &imageAcquiredSemaphore);
				assert(res == VK_SUCCESS);

				// Get the index of the next available swapchain image:
				res = vkAcquireNextImageKHR(this->info.device, info.swap_chain, UINT64_MAX, imageAcquiredSemaphore, VK_NULL_HANDLE,
											&this->info.current_buffer);
				// TODO: Deal with the VK_SUBOPTIMAL_KHR and VK_ERROR_OUT_OF_DATE_KHR
				// return codes
				assert(res == VK_SUCCESS);

				VkRenderPassBeginInfo rp_begin;
				rp_begin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
				rp_begin.pNext = NULL;
				rp_begin.renderPass =  this->info.render_pass;
				rp_begin.framebuffer = this->info.framebuffers[info.current_buffer];
				rp_begin.renderArea.offset.x = 0;
				rp_begin.renderArea.offset.y = 0;
				rp_begin.renderArea.extent.width = (uint32_t)this->info.width;
				rp_begin.renderArea.extent.height = (uint32_t)this->info.height;
				rp_begin.clearValueCount = 2;
				rp_begin.pClearValues = clear_values;

				vkCmdBeginRenderPass(this->info.cmd, &rp_begin, VK_SUBPASS_CONTENTS_INLINE);

				vkCmdBindPipeline(this->info.cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, this->info.pipeline);
				vkCmdBindDescriptorSets(this->info.cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, this->info.pipeline_layout, 0, 1,
										this->info.desc_set.data(), 0, NULL);

				const VkDeviceSize offsets[1] = {0};
				vkCmdBindVertexBuffers(this->info.cmd, 0, 1, &this->info.vertex_buffer.buf, offsets);

				this->init_viewports();
				this->init_scissors();

				vkCmdDraw(this->info.cmd, 12 * 3, 1, 0, 0);
				vkCmdEndRenderPass(this->info.cmd);
				res = vkEndCommandBuffer(this->info.cmd);
				const VkCommandBuffer cmd_bufs[] = {this->info.cmd};
				VkFenceCreateInfo fenceInfo;
				VkFence drawFence;
				fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
				fenceInfo.pNext = NULL;
				fenceInfo.flags = 0;
				vkCreateFence(info.device, &fenceInfo, NULL, &drawFence);

				VkPipelineStageFlags pipe_stage_flags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				VkSubmitInfo submit_info[1] = {};
				submit_info[0].pNext = NULL;
				submit_info[0].sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
				submit_info[0].waitSemaphoreCount = 1;
				submit_info[0].pWaitSemaphores = &imageAcquiredSemaphore;
				submit_info[0].pWaitDstStageMask = &pipe_stage_flags;
				submit_info[0].commandBufferCount = 1;
				submit_info[0].pCommandBuffers = cmd_bufs;
				submit_info[0].signalSemaphoreCount = 0;
				submit_info[0].pSignalSemaphores = NULL;

				/* Queue the command buffer for execution */
				res = vkQueueSubmit(this->info.graphics_queue, 1, submit_info, drawFence);
				assert(res == VK_SUCCESS);

				/* Now present the image in the window */

				VkPresentInfoKHR present;
				present.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
				present.pNext = NULL;
				present.swapchainCount = 1;
				present.pSwapchains = &this->info.swap_chain;
				present.pImageIndices = &this->info.current_buffer;
				present.pWaitSemaphores = NULL;
				present.waitSemaphoreCount = 0;
				present.pResults = NULL;

				/* Make sure command buffer is finished before presenting */
				do {
					res = vkWaitForFences(this->info.device, 1, &drawFence, VK_TRUE, VK_SAMPLE_COUNT_1_BIT);
				} while (res == VK_TIMEOUT);

				assert(res == VK_SUCCESS);
				res = vkQueuePresentKHR(this->info.present_queue, &present);
				assert(res == VK_SUCCESS);

				wait_seconds(1);
				if (this->info.save_images) write_ppm("drawcube");

				vkDestroySemaphore(info.device, imageAcquiredSemaphore, NULL);
				vkDestroyFence(info.device, drawFence, NULL);
			}

		public:

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
