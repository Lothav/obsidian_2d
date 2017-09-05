//
// Created by luizorv on 6/10/17.
//

#ifndef OBSIDIAN2D_CORE_UTIL_H
#define OBSIDIAN2D_CORE_UTIL_H

#include <assert.h>
#include <fstream>
#include "./vulkan/vulkan.h"

#if defined(NDEBUG) && defined(__GNUC__)
#define U_ASSERT_ONLY __attribute__((unused))
#else
#define U_ASSERT_ONLY
#endif



#include "Obsidian2D/Util/Loggable.h"

#endif //OBSIDIAN2D_CORE_UTIL_H
namespace Obsidian2D
{
	namespace Renderer
	{
		class Util
		{
		public:


			static void init_viewports(VkCommandBuffer cmd_buffer, float width, float height)
			{
				VkViewport viewport;
				viewport.height 		= height;
				viewport.width 			= width;
				viewport.minDepth 		= (float)0.0f;
				viewport.maxDepth 		= (float)1.0f;
				viewport.x 				= 0;
				viewport.y 				= 0;

				vkCmdSetViewport(cmd_buffer, 0, 1, &viewport);
			}

			static void init_scissors(VkCommandBuffer cmd_buffer, uint32_t width, uint32_t height)
			{
				VkRect2D scissor;
				scissor.extent.width 	= (uint32_t)width;
				scissor.extent.height 	= (uint32_t)height;
				scissor.offset.x 		= 0;
				scissor.offset.y 		= 0;

				vkCmdSetScissor(cmd_buffer, 0, 1, &scissor);
			}

			static VkShaderModule loadSPIRVShader(std::string filename, VkDevice device)
			{
				size_t shaderSize;
				char* shaderCode = nullptr;

				std::ifstream is(getAssetPath() + filename, std::ios::binary | std::ios::in | std::ios::ate);

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

			static const std::string getAssetPath()
			{
				return "./../../include/Obsidian2D/Renderer/";
			}

			void wait_seconds(int seconds)
			{
				//sleep(seconds);
			}
		};
	}
}
