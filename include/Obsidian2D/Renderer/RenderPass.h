//
// Created by luizorv on 9/3/17.
//

#ifndef OBSIDIAN2D_RENDERPASS_H
#define OBSIDIAN2D_RENDERPASS_H

#include <Obsidian2D/Renderer/vulkan/vulkan.h>
#include <vector>
#include <array>
#include <assert.h>

struct rpAttachments {
	VkFormat format;
	bool clear;
};

namespace Obsidian2D
{
	namespace Renderer
	{
		class RenderPass {

		private:

			VkDevice 			_instance_device;
			VkRenderPass 		_render_pass;

		public:

			RenderPass(VkDevice device)
			{
				_instance_device = device;
			}

			void createRenderPass(std::vector<struct rpAttachments> att_vector)
			{
				const VkSubpassDescription 		subpassDescription 	  = getSubPass();
				const VkAttachmentDescription* 	attachmentDescription = getSubAttachments(att_vector).data();

				VkRenderPassCreateInfo rp_info = {};
				rp_info.sType 								= VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
				rp_info.pNext 								= NULL;
				rp_info.attachmentCount 					= 2;
				rp_info.pAttachments 						= attachmentDescription;
				rp_info.subpassCount 						= 1;
				rp_info.pSubpasses 							= &subpassDescription;
				rp_info.dependencyCount 					= 0;
				rp_info.pDependencies 						= NULL;

				assert(vkCreateRenderPass(_instance_device, &rp_info, NULL, &_render_pass) == VK_SUCCESS);
			}

			VkRenderPass getRenderPass()
			{
				return _render_pass;
			}

		private:

			VkSubpassDescription getSubPass()
			{

				VkAttachmentReference color_reference = {};
				color_reference.attachment 					= 0;
				color_reference.layout 						= VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

				VkAttachmentReference depth_reference = {};
				depth_reference.attachment 					= 1;
				depth_reference.layout 						= VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

				VkSubpassDescription subpass = {};
				subpass.pipelineBindPoint 					= VK_PIPELINE_BIND_POINT_GRAPHICS;
				subpass.flags 								= 0;
				subpass.inputAttachmentCount 				= 0;
				subpass.pInputAttachments 					= nullptr;
				subpass.colorAttachmentCount 				= 1;
				subpass.pColorAttachments 					= &color_reference;
				subpass.pResolveAttachments 				= nullptr;
				subpass.pDepthStencilAttachment 			= &depth_reference;
				subpass.preserveAttachmentCount 			= 0;
				subpass.pPreserveAttachments 				= nullptr;

				return subpass;
			}

			std::array<VkAttachmentDescription, att_vector.size()> getSubAttachments(std::vector<struct rpAttachments> att_vector) {
				std::array<VkAttachmentDescription, att_vector.size()> attachments;
				int i = 0;
				for (; i < att_vector.size(); i++) {
					attachments[i].format = att_vector[i].format;
					attachments[i].samples = VK_SAMPLE_COUNT_1_BIT;
					attachments[i].loadOp = att_vector[i].clear ? VK_ATTACHMENT_LOAD_OP_CLEAR
																: VK_ATTACHMENT_LOAD_OP_DONT_CARE;
					attachments[i].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
					attachments[i].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
					attachments[i].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
					attachments[i].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
					attachments[i].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
					attachments[i].flags = 0;
				}
				return attachments;
			}
		};
	}
}
#endif //OBSIDIAN2D_RENDERPASS_H
