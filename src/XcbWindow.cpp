#include <cstdlib>
#include <cstring>

#include <iostream>
#include <assert.h>
#include "Obsidian2D/Renderer/XcbWindow.h"
#include <bitset>
using namespace Obsidian2D::Renderer;

void XcbWindow::bootstrap(){
	VkResult set_global_layer = this->setGlobalLayerProperties(this->info);

	VkApplicationInfo app_info = this->setApplicationInfo();
	VkResult set_instance_info = this->setInstanceInfo(app_info);

	if(set_instance_info == VK_SUCCESS && set_global_layer == VK_SUCCESS){
		uint32_t gpu_count = 1;
		this->enumerateDevice(gpu_count);
		this->createDevice();
		this->createCommandBuffer();
		this->setWindowSize();
		this->setXCBConnection();
		this->createWindow();
		this->initSwapChainExtension();
	} else {
		//@TODO throw error
	}

}

void XcbWindow::setWindowSize(){
#ifdef __ANDROID__
	AndroidGetWindowSize(&this->info.width, &this->info.height);
#else
	this->info.width = this->width;
	this->info.height = this->height;
#endif
}

void XcbWindow::setXCBConnection() {
#if !(defined(_WIN32) || defined(__ANDROID__) || defined(VK_USE_PLATFORM_IOS_MVK) || defined(VK_USE_PLATFORM_MACOS_MVK))
// Do nothing on Android, Apple, or Windows.
	const xcb_setup_t *setup;
	xcb_screen_iterator_t iter;
	int scr;

	this->info.connection = xcb_connect(NULL, &scr);
	if (this->info.connection == NULL || xcb_connection_has_error(this->info.connection)) {
		std::cout << "Unable to make an XCB connection\n";
		exit(-1);
	}

	setup = xcb_get_setup(this->info.connection);
	iter = xcb_setup_roots_iterator(setup);
	while (scr-- > 0) xcb_screen_next(&iter);

	this->info.screen = iter.data;
#endif
}

void XcbWindow::createWindow() {
	assert(this->info.width > 0);
	assert(this->info.height > 0);

	uint32_t value_mask, value_list[32];

	this->info.window = xcb_generate_id(this->info.connection);

	value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
	value_list[0] = this->info.screen->black_pixel;
	value_list[1] = XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_BUTTON_PRESS |
					XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_ENTER_WINDOW | XCB_EVENT_MASK_LEAVE_WINDOW;

	xcb_create_window(this->info.connection, XCB_COPY_FROM_PARENT, this->info.window, this->info.screen->root,
					  0, 0, (uint16_t)this->info.width, (uint16_t)this->info.height, 0,
					  XCB_WINDOW_CLASS_INPUT_OUTPUT, this->info.screen->root_visual, value_mask, value_list);

	/* Magic code that will send notification when window is destroyed */
	xcb_intern_atom_cookie_t cookie = xcb_intern_atom(this->info.connection, 1, 12, "WM_PROTOCOLS");
	xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(this->info.connection, cookie, 0);

	xcb_intern_atom_cookie_t cookie2 = xcb_intern_atom(this->info.connection, 0, 16, "WM_DELETE_WINDOW");
	this->info.atom_wm_delete_window = xcb_intern_atom_reply(this->info.connection, cookie2, 0);

	xcb_change_property(this->info.connection, XCB_PROP_MODE_REPLACE, this->info.window, (*reply).atom, 4, 32, 1,
						&(*this->info.atom_wm_delete_window).atom);
	free(reply);

	xcb_change_property(this->info.connection, XCB_PROP_MODE_REPLACE, this->info.window,
		XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8, strlen(APP_NAME), APP_NAME);

	xcb_flush(this->info.connection); 

	xcb_map_window(this->info.connection, this->info.window);

	// Force the x/y coordinates to 100, 100 results are identical in consecutive
	// runs
	const uint32_t coords[] = {100, 100};
	xcb_configure_window(this->info.connection, this->info.window, XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y, coords);
	xcb_flush(this->info.connection);

}

void XcbWindow::destroyWindow() {
	vkDestroySurfaceKHR(this->info.inst, this->info.surface, NULL);
	xcb_destroy_window(this->info.connection, this->info.window);
	xcb_disconnect(this->info.connection);
}

void XcbWindow::initSwapChainExtension() {
	VkResult res;
#ifdef _WIN32
	VkWin32SurfaceCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	createInfo.pNext = NULL;
	createInfo.hinstance = this->info.connection;
	createInfo.hwnd = this->info.window;
	res = vkCreateWin32SurfaceKHR(this->info.inst, &createInfo, NULL, &this->info.surface);
#elif defined(__ANDROID__)
	GET_INSTANCE_PROC_ADDR(this->info.inst, CreateAndroidSurfaceKHR);

	VkAndroidSurfaceCreateInfoKHR createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
	createInfo.pNext = nullptr;
	createInfo.flags = 0;
	createInfo.window = AndroidGetApplicationWindow();
	res = info.fpCreateAndroidSurfaceKHR(this->info.inst, &createInfo, nullptr, &this->info.surface);
#elif defined(VK_USE_PLATFORM_IOS_MVK)
	VkIOSSurfaceCreateInfoMVK createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_IOS_SURFACE_CREATE_INFO_MVK;
	createInfo.pNext = NULL;
	createInfo.flags = 0;
	createInfo.pView = this->info.window;
	res = vkCreateIOSSurfaceMVK(this->info.inst, &createInfo, NULL, &this->info.surface);
#elif defined(VK_USE_PLATFORM_MACOS_MVK)
	VkMacOSSurfaceCreateInfoMVK createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK;
	createInfo.pNext = NULL;
	createInfo.flags = 0;
	createInfo.pView = this->info.window;
	res = vkCreateMacOSSurfaceMVK(this->info.inst, &createInfo, NULL, &this->info.surface);
#else
	VkXcbSurfaceCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
	createInfo.pNext = NULL;
	createInfo.connection = this->info.connection;
	createInfo.window = this->info.window;
	res = vkCreateXcbSurfaceKHR(this->info.inst, &createInfo, NULL, &this->info.surface);
	std::cout << "patch" << std::endl;
#endif  // __ANDROID__  && _WIN32

	std::cout << res << std::endl;
	//@TODO FIX THIS ERROR !!!! >> VK_ERROR_EXTENSION_NOT_PRESENT
	//assert(res == VK_SUCCESS);
}