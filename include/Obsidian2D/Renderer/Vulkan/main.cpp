#include <malloc.h>
#include "main.h"

void Obsidian2D::Renderer::Vulkan::Main::bootstrap(){
	this->setGlobalLayerProperties(this->info);
}

VkResult Obsidian2D::Renderer::Vulkan::Main::setGlobalLayerProperties(struct VulkanInfo &info) {

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

VkResult Obsidian2D::Renderer::Vulkan::Main::setGlobalExtensionProperties(layer_properties &layer_props) {
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
