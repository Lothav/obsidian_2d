#include <malloc.h>
#include "Obsidian2D/Renderer/instance.h"
#include <assert.h>

using namespace Obsidian2D::Renderer;

void Instance::bootstrap(){
	this->setGlobalLayerProperties(this->info);
	VkApplicationInfo app_info = this->setApplicationInfo();
	VkResult set_instance_info = this->setInstanceInfo(app_info);

	if(set_instance_info == VK_SUCCESS){
		uint32_t gpu_count = 1;
		VkResult enum_devices = this->enumerateDevice(this->info, gpu_count);

		//////

	} else {
		//@TODO throw error
	}

}

VkResult Instance::setGlobalLayerProperties(struct VulkanInfo &info) {

	uint32_t instance_layer_count;
	VkLayerProperties *vk_props = NULL;
	VkResult res;
#ifdef __ANDROID__
	// This place is the first place for samples to use Vulkan APIs.
    // Here, we are going to open Vulkan.so on the device and retrieve function pointers using
    // vulkan_wrapper helper.
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

	return res;
}

VkResult Instance::setGlobalExtensionProperties(layer_properties &layer_props) {
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

VkApplicationInfo Instance::setApplicationInfo(){

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

VkResult Instance::setInstanceInfo(VkApplicationInfo app_info){
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

VkResult Instance::enumerateDevice(struct VulkanInfo &info, uint32_t gpu_count) {
	uint32_t const req_count = gpu_count;
	VkResult res = vkEnumeratePhysicalDevices(info.inst, &gpu_count, NULL);
	assert(gpu_count);
	info.gpus.resize(gpu_count);

	res = vkEnumeratePhysicalDevices(info.inst, &gpu_count, info.gpus.data());
	assert(!res && gpu_count >= req_count);

	vkGetPhysicalDeviceQueueFamilyProperties(info.gpus[0], &info.queue_family_count, NULL);
	assert(info.queue_family_count >= 1);

	info.queue_props.resize(info.queue_family_count);
	vkGetPhysicalDeviceQueueFamilyProperties(info.gpus[0], &info.queue_family_count, info.queue_props.data());
	assert(info.queue_family_count >= 1);

	/* This is as good a place as any to do this */
	vkGetPhysicalDeviceMemoryProperties(info.gpus[0], &info.memory_properties);
	vkGetPhysicalDeviceProperties(info.gpus[0], &info.gpu_props);

	return res;
}