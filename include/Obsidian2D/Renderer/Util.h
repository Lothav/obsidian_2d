//
// Created by luizorv on 6/10/17.
//

#ifndef OBSIDIAN2D_CORE_UTIL_H
#define OBSIDIAN2D_CORE_UTIL_H
#include "./vulkan/vulkan.h"

#if defined(NDEBUG) && defined(__GNUC__)
#define U_ASSERT_ONLY __attribute__((unused))
#else
#define U_ASSERT_ONLY
#endif

#include <vulkan/vulkan.h>
#include <xcb/xcb.h>
#include <vector>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <cstring>

#include "Obsidian2D/Util/Loggable.h"

struct Vertex {
	float pos[3];
	float uv[2];
	float normal[3];
};
std::vector<Vertex> vertexData =
{
		{ { -1.0f,  1.0f, 0.0f }, { 0.0f, 1.0f },{ 0.0f, 0.0f, 1.0f } },
		{ { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f },{ 0.0f, 0.0f, 1.0f } },
        { { 1.0f,  1.0f, 0.0f }, { 1.0f, 1.0f },{ 0.0f, 0.0f, 1.0f } },

        { { 1.0f, 1.0f, 0.0f }, { 1.0f, 1.0f },{ 0.0f, 0.0f, 1.0f } },
        { { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f },{ 0.0f, 0.0f, 1.0f } },
        { { 1.0f,  -1.0f, 0.0f }, { 1.0f, 0.0f },{ 0.0f, 0.0f, 1.0f } },
};

typedef struct _swap_chain_buffers {
	VkImage image;
	VkImageView view;
} swap_chain_buffer;

#endif //OBSIDIAN2D_CORE_UTIL_H
namespace Obsidian2D
{
	namespace Renderer
	{
		class Util : public Obsidian2D::Util::Loggable
		{
		protected:
			int32_t									width;
			int32_t									height;


			void init_viewports(VkCommandBuffer cmd_buffer)
			{
#ifdef __ANDROID__
				// Disable dynamic viewport on Android. Some drive has an issue with the dynamic viewport
// feature.
#else
				VkViewport viewport;
				viewport.height = (float)height;
				viewport.width = (float)width;
				viewport.minDepth = (float)0.0f;
				viewport.maxDepth = (float)1.0f;
				viewport.x = 0;
				viewport.y = 0;
				vkCmdSetViewport(cmd_buffer, 0, 1, &viewport);
#endif
			}

			void init_scissors(VkCommandBuffer cmd_buffer)
			{
#ifdef __ANDROID__
				// Disable dynamic viewport on Android. Some drive has an issue with the dynamic scissors feature.
#else
				VkRect2D scissor;
				scissor.extent.width =  (uint32_t)width;
				scissor.extent.height = (uint32_t)height;
				scissor.offset.x = 0;
				scissor.offset.y = 0;
				vkCmdSetScissor(cmd_buffer, 0, 1, &scissor);
#endif
			}

			VkShaderModule loadSPIRVShader(std::string filename, VkDevice device)
			{
				size_t shaderSize;
				char* shaderCode = nullptr;

#if defined(__ANDROID__)
				// Load shader from compressed asset
		AAsset* asset = AAssetManager_open(androidApp->activity->assetManager, filename.c_str(), AASSET_MODE_STREAMING);
		assert(asset);
		shaderSize = AAsset_getLength(asset);
		assert(shaderSize > 0);

		shaderCode = new char[shaderSize];
		AAsset_read(asset, shaderCode, shaderSize);
		AAsset_close(asset);
#else
				std::ifstream is(filename, std::ios::binary | std::ios::in | std::ios::ate);

				if (is.is_open())
				{
					shaderSize = is.tellg();
					is.seekg(0, std::ios::beg);
					// Copy file contents into a buffer
					shaderCode = new char[shaderSize];
					is.read(shaderCode, shaderSize);
					is.close();
					assert(shaderSize > 0);
				}
#endif
				if (shaderCode)
				{
					// Create a new shader module that will be used for pipeline creation
					VkShaderModuleCreateInfo moduleCreateInfo{};
					moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
					moduleCreateInfo.codeSize = shaderSize;
					moduleCreateInfo.pCode = (uint32_t*)shaderCode;

					VkShaderModule shaderModule;
					VkResult res = vkCreateShaderModule(device, &moduleCreateInfo, NULL, &shaderModule);
					assert(res == VK_SUCCESS);

					delete[] shaderCode;

					return shaderModule;
				}
				else
				{
					std::cerr << "Error: Could not open shader file \"" << filename << "\"" << std::endl;
					return VK_NULL_HANDLE;
				}
			}

			const std::string getAssetPath()
			{
#if defined(__ANDROID__)
				return "";
#else
				return "./../../include/Obsidian2D/Renderer/";
#endif
			}

			void wait_seconds(int seconds)
			{
#ifdef WIN32
				Sleep(seconds * 1000);
#elif defined(__ANDROID__)
				sleep(seconds);
#else
				//sleep(seconds);
#endif
			}
		};
	}
}
