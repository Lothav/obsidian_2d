//
// Created by luizorv on 9/7/17.
//

#ifndef OBSIDIAN2D_SYNCPRIMITIVES_H
#define OBSIDIAN2D_SYNCPRIMITIVES_H

#include <vector>
#include <Obsidian2D/Renderer/vulkan/vulkan.h>

namespace Obsidian2D
{
	namespace Renderer
	{
		class SyncPrimitives
		{
		private:

			VkDevice 						_instance_device;
			std::vector<VkSemaphore> 		_semaphores = {};
			std::vector<VkFence> 			_fences = {};

		public:

			SyncPrimitives(VkDevice device)
			{
				_instance_device = device;
			}

			void createSemaphore(){}

			void createFence()
			{

				_fences.resize(_fences.size() + 1);

				VkFenceCreateInfo fenceInfo;
				fenceInfo.sType 			= VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
				fenceInfo.pNext 			= NULL;
				fenceInfo.flags 			= 0;

				vkCreateFence(_instance_device, &fenceInfo, NULL, &_fences[_fences.size()-1]);
			}

			VkFence* getFence(u_int32_t i)
			{
				return &this->_fences[i];
			}
		};
	}
}

#endif //OBSIDIAN2D_SYNCPRIMITIVES_H
