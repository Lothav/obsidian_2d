#ifndef OBSIDIAN2D_RENDERER_XCBWINDOW_
#define OBSIDIAN2D_RENDERER_XCBWINDOW_

#define XYZ1(_x_, _y_, _z_) (_x_), (_y_), (_z_), 1.0

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <bitset>
#include <xcb/xcb.h>

#include "Obsidian2D/Renderer/Window.h"
#include "Obsidian2D/Core/WindowEvent.h"

struct Vertex {
	float posX, posY, posZ, posW;  // Position data
	float r, g, b, a;              // Color
};

struct VertexUV {
	float posX, posY, posZ, posW;  // Position data
	float u, v;                    // texture u,v
};


static const VertexUV g_vb_texture_Data[] = {
		{XYZ1(-1, -1, 1)},
		{XYZ1( 1, -1, 1)},
		{XYZ1( 1,  1, 1)},
		{XYZ1(-1, -1, 1)},
		{XYZ1( 1,  1, 1)},
		{XYZ1(-1,  1, 1)}
};

namespace Obsidian2D
{
	namespace Renderer
	{
		class XcbWindow: public Window
		{
		public:

			~XcbWindow(){
				/*vkDestroyPipeline(device, vkPipeline, NULL);
				vkDestroyPipelineCache(device, pPipelineCache, NULL);
				vkDestroyDescriptorPool(device, desc_pool, NULL);

				// Destroy vertex buffer
				vkDestroyBuffer(device, vertex_buffer.buf, NULL);
				vkFreeMemory(device, vertex_buffer.mem, NULL);

				// Destroy frame buffer
				for (uint32_t i = 0; i < swapchainImageCount; i++) {
					vkDestroyFramebuffer(device, framebuffers[i], NULL);
				}
				free(framebuffers);

				// Destroy shaders
				vkDestroyShaderModule(device, shaderStages[0].module, NULL);
				vkDestroyShaderModule(device, shaderStages[1].module, NULL);

				// Destroy render pass
				vkDestroyRenderPass(device, info.render_pass, NULL);

				// Destroy descriptor and pipeline layouts
				for (int i = 0; i < 1; i++) vkDestroyDescriptorSetLayout(device, desc_layout[i], NULL);
				vkDestroyPipelineLayout(device, pipeline_layout, NULL);

				// Destroy uniform buffer
				vkDestroyBuffer(device, uniform_data.buf, NULL);
				vkFreeMemory(device, uniform_data.mem, NULL);

				// Destroy depth buffer
				vkDestroyImageView(device, depth.view, NULL);
				vkDestroyImage(device, depth.image, NULL);
				vkFreeMemory(device, depth.mem, NULL);

				// Destroy Swap Chain
				for (uint32_t i = 0; i < info.swapchainImageCount; i++) {
					vkDestroyImageView(info.device, info.buffers[i].view, NULL);
				}
				vkDestroySwapchainKHR(info.device, info.swap_chain, NULL);

				// Destroy Command Buffers
				VkCommandBuffer cmd_bufs[1] = {cmd};
				vkFreeCommandBuffers(device, cmd_pool, 1, cmd_bufs);
				vkDestroyCommandPool(device, cmd_pool, NULL);

				//vkDestroyDevice(this->device, NULL);

				// Detroy Window
				vkDestroySurfaceKHR(inst, surface, NULL);
				xcb_destroy_window(connection, window);
				xcb_disconnect(connection);

				vkDestroyInstance(inst, NULL);*/
			}
		private:

			xcb_connection_t *connection;
			xcb_screen_t *screen;
			xcb_window_t window;
			uint32_t graphics_queue_family_index = UINT32_MAX;
			uint32_t present_queue_family_index = UINT32_MAX;
			VkSurfaceKHR surface;



			void setXCBConnection() // for Linux users :V
			{
#if !(defined(_WIN32) || defined(__ANDROID__) || defined(VK_USE_PLATFORM_IOS_MVK) || defined(VK_USE_PLATFORM_MACOS_MVK))
				// Do nothing on Android, Apple, or Windows.
				const xcb_setup_t *setup;
				xcb_screen_iterator_t iter;
				int scr;

				connection = xcb_connect(NULL, &scr);
				if (connection == NULL || xcb_connection_has_error(connection)) {
					std::cout << "Unable to make an XCB connection\n";
					exit(-1);
				}

				setup = xcb_get_setup(connection);
				iter = xcb_setup_roots_iterator(setup);
				while (scr-- > 0) xcb_screen_next(&iter);

				screen = iter.data;
#endif
			}

			void createWindow()
			{
				assert(this->width > 0);
				assert(this->height > 0);

				uint32_t value_mask, value_list[32];

				window = xcb_generate_id(connection);

				value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
				value_list[0] = screen->black_pixel;
				value_list[1] = XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_BUTTON_PRESS |
								XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_ENTER_WINDOW | XCB_EVENT_MASK_LEAVE_WINDOW;

				xcb_create_window(connection, XCB_COPY_FROM_PARENT, window, screen->root,
								  0, 0, (uint16_t)this->width, (uint16_t)this->height, 0,
								  XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual, value_mask, value_list);

				/* Magic code that will send notification when window is destroyed */
				xcb_intern_atom_cookie_t cookie = xcb_intern_atom(connection, 1, 12, "WM_PROTOCOLS");
				xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(connection, cookie, 0);

				xcb_intern_atom_cookie_t cookie2 = xcb_intern_atom(connection, 0, 16, "WM_DELETE_WINDOW");
				xcb_intern_atom_reply_t *atom_wm_delete_window = xcb_intern_atom_reply(connection, cookie2, 0);

				xcb_change_property(connection, XCB_PROP_MODE_REPLACE, window, (*reply).atom, 4, 32, 1,
									&(*atom_wm_delete_window).atom);
				free(reply);

				xcb_change_property(connection, XCB_PROP_MODE_REPLACE, window,
									XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8, strlen(APP_NAME), APP_NAME);

				xcb_flush(connection);

				xcb_map_window(connection, window);

				// Force the x/y coordinates to 100, 100 results are identical in consecutive
				// runs
				const uint32_t coords[] = {100, 100};
				xcb_configure_window(connection, window, XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y, coords);
				xcb_flush(connection);
			}

			void initSwapChainExtension()
			{
				VkResult res;

#ifdef _WIN32
				VkWin32SurfaceCreateInfoKHR createInfo = {};
				createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
				createInfo.pNext = NULL;
				createInfo.hinstance = connection;
				createInfo.hwnd = window;
				res = vkCreateWin32SurfaceKHR(inst, &createInfo, NULL, &surface);
#elif defined(__ANDROID__)
				GET_INSTANCE_PROC_ADDR(inst, CreateAndroidSurfaceKHR);

				VkAndroidSurfaceCreateInfoKHR createInfo;
				createInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
				createInfo.pNext = nullptr;
				createInfo.flags = 0;
				createInfo.window = AndroidGetApplicationWindow();
				res = info.fpCreateAndroidSurfaceKHR(inst, &createInfo, nullptr, &surface);
#elif defined(VK_USE_PLATFORM_IOS_MVK)
				VkIOSSurfaceCreateInfoMVK createInfo = {};
				createInfo.sType = VK_STRUCTURE_TYPE_IOS_SURFACE_CREATE_INFO_MVK;
				createInfo.pNext = NULL;
				createInfo.flags = 0;
				createInfo.pView = window;
				res = vkCreateIOSSurfaceMVK(inst, &createInfo, NULL, &surface);
#elif defined(VK_USE_PLATFORM_MACOS_MVK)
				VkMacOSSurfaceCreateInfoMVK createInfo = {};
				createInfo.sType = VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK;
				createInfo.pNext = NULL;
				createInfo.flags = 0;
				createInfo.pView = window;
				res = vkCreateMacOSSurfaceMVK(inst, &createInfo, NULL, &surface);
#else
				VkXcbSurfaceCreateInfoKHR createInfo = {};
				createInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
				createInfo.pNext = NULL;
				createInfo.connection = connection;
				createInfo.window = window;
				res = vkCreateXcbSurfaceKHR(instance, &createInfo, NULL, &surface);
#endif  // __ANDROID__  && _WIN32

				assert(res == VK_SUCCESS);

				VkBool32 *pSupportsPresent = (VkBool32 *)malloc(queue_family_count * sizeof(VkBool32));
				for (uint32_t i = 0; i < queue_family_count; i++) {
					vkGetPhysicalDeviceSurfaceSupportKHR(gpu_vector[0], i, surface, &pSupportsPresent[i]);
				}

				// Search for a graphics and a present queue in the array of queue
				// families, try to find one that supports both

				for (uint32_t i = 0; i < queue_family_count; ++i) {
					if ((queue_family_props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) {
						if (graphics_queue_family_index == UINT32_MAX) graphics_queue_family_index = i;

						if (pSupportsPresent[i] == VK_TRUE) {
							graphics_queue_family_index = i;
							present_queue_family_index = i;
							break;
						}
					}
				}

				if (present_queue_family_index == UINT32_MAX) {
					// If didn't find a queue that supports both graphics and present, then
					// find a separate present queue.
					for (size_t i = 0; i < queue_family_count; ++i)
						if (pSupportsPresent[i] == VK_TRUE) {
							present_queue_family_index = (u_int32_t)i;
							break;
						}
				}
				free(pSupportsPresent);

				// Generate error if could not find queues that support graphics
				// and present
				if (graphics_queue_family_index == UINT32_MAX || present_queue_family_index == UINT32_MAX) {
					std::cout << "Could not find a queues for both graphics and present";
					exit(-1);
				}

				// Get the list of VkFormats that are supported:
				uint32_t formatCount;
				res = vkGetPhysicalDeviceSurfaceFormatsKHR(gpu_vector[0], surface, &formatCount, NULL);
				assert(res == VK_SUCCESS);
				VkSurfaceFormatKHR *surfFormats = (VkSurfaceFormatKHR *)malloc(formatCount * sizeof(VkSurfaceFormatKHR));
				res = vkGetPhysicalDeviceSurfaceFormatsKHR(gpu_vector[0], surface, &formatCount, surfFormats);
				assert(res == VK_SUCCESS);
				// If the format list includes just one entry of VK_FORMAT_UNDEFINED,
				// the surface has no preferred format.  Otherwise, at least one
				// supported format will be returned.
				if (formatCount == 1 && surfFormats[0].format == VK_FORMAT_UNDEFINED) {
					format = VK_FORMAT_B8G8R8A8_UNORM;
				} else {
					assert(formatCount >= 1);
					format = surfFormats[0].format;
				}
				free(surfFormats);
			}
		public:
			XcbWindow(int32_t width, int32_t height)
			{
#ifdef __ANDROID__
				AndroidGetWindowSize(&width, &height);
#else
				this->width = width;
				this->height = height;
#endif
			}

			void bootstrap()
			{
				const bool depthPresent = false;

				this->createInstance();
				this->createLogicalDeviceAndCommandBuffer();
				this->setXCBConnection();
				this->createWindow();
				this->initSwapChainExtension();
				this->initBuffers(
						graphics_queue_family_index,
						present_queue_family_index,
						surface,
						depthPresent,
						g_vb_texture_Data,
						sizeof(g_vb_texture_Data),
						sizeof(g_vb_texture_Data[0])
				);
			}

			int y = 0, x =0;
			int c_x = 0, c_y = 0;

			::Obsidian2D::Core::WindowEvent poolEvent()
			{
				xcb_generic_event_t* e = nullptr;
				xcb_intern_atom_reply_t *atom_wm_delete_window;

				while ((e = xcb_poll_for_event(connection))) {
					if ((e->response_type & ~0x80) == XCB_CLIENT_MESSAGE) {
						if((*(xcb_client_message_event_t*)e).data.data32[0] == (*atom_wm_delete_window).atom) {
							return ::Obsidian2D::Core::WindowEvent::Close;
						}
					} else if((e->response_type & ~0x80) == XCB_ENTER_NOTIFY) {
						return ::Obsidian2D::Core::WindowEvent::Focus;
					} else if((e->response_type & ~0x80) == XCB_LEAVE_NOTIFY) {
						return ::Obsidian2D::Core::WindowEvent::Blur;
					} else if((e->response_type & ~0x80) == XCB_BUTTON_PRESS) {
						return ::Obsidian2D::Core::WindowEvent::Click;
					} else if((e->response_type & ~0x80) == XCB_BUTTON_RELEASE) {
						//return ::Obsidian2D::Core::WindowEvent::ClickEnd;
					} else if((e->response_type & ~0x80) == XCB_KEY_PRESS) {

						VkResult U_ASSERT_ONLY res;

						xcb_key_press_event_t * kp = (xcb_key_press_event_t *)e;

						if( kp->detail == 'O'){
							x--;
						}else if(kp->detail == 'P'){
							y++;
						}else if(kp->detail == 'Q'){
							x++;
						}else if(kp->detail == 'j'){
							y--;
						}

						if( kp->detail == 'W'){
							c_x--;
						}else if(kp->detail == 'X'){
							c_y--;
						}else if(kp->detail == 'Y'){
							c_x++;
						}else if(kp->detail == 'T'){
							c_y++;
						}

						std::cout << kp->detail << std::endl;
						this->setCameraViewCenter(glm::vec3(x, y, 0));
						this->setCameraViewEye(glm::vec3(c_x, c_y, -10));
						this->updateCamera();

						this->draw();

						return ::Obsidian2D::Core::WindowEvent::ButtonDown;
					} else if((e->response_type & ~0x80) == XCB_KEY_RELEASE) {
						return ::Obsidian2D::Core::WindowEvent::ButtonUp;
					} else {
						return ::Obsidian2D::Core::WindowEvent::Unknow;
					}
				}

				return ::Obsidian2D::Core::WindowEvent::None;
			}
		};
	}
}

#endif //OBSIDIAN2D_RENDERER_XCBWINDOW_
