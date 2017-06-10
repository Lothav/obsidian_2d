//
// Created by luizorv on 6/10/17.
//

#ifndef OBSIDIAN2D_CORE_PIPELINE_H
#define OBSIDIAN2D_CORE_PIPELINE_H

#include "Shaders.h"

namespace Obsidian2D
{
	namespace Renderer
	{
		class Pipeline : public Shaders
		{

		private:
			void initDescriptorPool(bool use_texture)
			{
				/* DEPENDS on init_uniform_buffer() and
				 * init_descriptor_and_pipeline_layouts() */

				VkResult U_ASSERT_ONLY res;
				VkDescriptorPoolSize type_count[2];
				type_count[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				type_count[0].descriptorCount = 1;
				if (use_texture) {
					type_count[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					type_count[1].descriptorCount = 1;
				}

				VkDescriptorPoolCreateInfo descriptor_pool = {};
				descriptor_pool.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
				descriptor_pool.pNext = NULL;
				descriptor_pool.maxSets = 1;
				descriptor_pool.poolSizeCount = use_texture ? 2 : 1;
				descriptor_pool.pPoolSizes = type_count;

				res = vkCreateDescriptorPool(this->info.device, &descriptor_pool, NULL, &this->info.desc_pool);
				assert(res == VK_SUCCESS);
			}

			void initDescriptorSet(bool use_texture)
			{
				/* DEPENDS on init_descriptor_pool() */

				VkResult U_ASSERT_ONLY res;

				VkDescriptorSetAllocateInfo alloc_info[1];
				alloc_info[0].sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
				alloc_info[0].pNext = NULL;
				alloc_info[0].descriptorPool = this->info.desc_pool;
				alloc_info[0].descriptorSetCount = 1;
				alloc_info[0].pSetLayouts = this->info.desc_layout.data();

				this->info.desc_set.resize(1);
				res = vkAllocateDescriptorSets(this->info.device, alloc_info, this->info.desc_set.data());
				assert(res == VK_SUCCESS);

				VkWriteDescriptorSet writes[2];

				writes[0] = {};
				writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				writes[0].pNext = NULL;
				writes[0].dstSet = this->info.desc_set[0];
				writes[0].descriptorCount = 1;
				writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				writes[0].pBufferInfo = &this->info.uniform_data.buffer_info;
				writes[0].dstArrayElement = 0;
				writes[0].dstBinding = 0;

				if (use_texture) {
					writes[1] = {};
					writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					writes[1].dstSet = this->info.desc_set[0];
					writes[1].dstBinding = 1;
					writes[1].descriptorCount = 1;
					writes[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					writes[1].pImageInfo = &this->info.texture_data.image_info;
					writes[1].dstArrayElement = 0;
				}

				vkUpdateDescriptorSets(this->info.device, use_texture ? 2 : 1, writes, 0, NULL);
			}

			void initPipelineCache()
			{
				VkResult U_ASSERT_ONLY res;

				VkPipelineCacheCreateInfo pipelineCache;
				pipelineCache.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
				pipelineCache.pNext = NULL;
				pipelineCache.initialDataSize = 0;
				pipelineCache.pInitialData = NULL;
				pipelineCache.flags = 0;
				res = vkCreatePipelineCache(this->info.device, &pipelineCache, NULL, &this->info.pipelineCache);
				assert(res == VK_SUCCESS);
			}

			void initPipeline(VkBool32 include_depth, VkBool32 include_vi = true) {
				VkResult U_ASSERT_ONLY res;

				VkDynamicState dynamicStateEnables[VK_DYNAMIC_STATE_RANGE_SIZE];
				VkPipelineDynamicStateCreateInfo dynamicState = {};
				memset(dynamicStateEnables, 0, sizeof dynamicStateEnables);
				dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
				dynamicState.pNext = NULL;
				dynamicState.pDynamicStates = dynamicStateEnables;
				dynamicState.dynamicStateCount = 0;

				VkPipelineVertexInputStateCreateInfo vi;
				memset(&vi, 0, sizeof(vi));
				vi.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
				if (include_vi) {
					vi.pNext = NULL;
					vi.flags = 0;
					vi.vertexBindingDescriptionCount = 1;
					vi.pVertexBindingDescriptions = &this->info.vi_binding;
					vi.vertexAttributeDescriptionCount = 2;
					vi.pVertexAttributeDescriptions = this->info.vi_attribs;
				}
				VkPipelineInputAssemblyStateCreateInfo ia;
				ia.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
				ia.pNext = NULL;
				ia.flags = 0;
				ia.primitiveRestartEnable = VK_FALSE;
				ia.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

				VkPipelineRasterizationStateCreateInfo rs;
				rs.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
				rs.pNext = NULL;
				rs.flags = 0;
				rs.polygonMode = VK_POLYGON_MODE_FILL;
				rs.cullMode = VK_CULL_MODE_BACK_BIT;
				rs.frontFace = VK_FRONT_FACE_CLOCKWISE;
				rs.depthClampEnable = VK_FALSE;
				rs.rasterizerDiscardEnable = VK_FALSE;
				rs.depthBiasEnable = VK_FALSE;
				rs.depthBiasConstantFactor = 0;
				rs.depthBiasClamp = 0;
				rs.depthBiasSlopeFactor = 0;
				rs.lineWidth = 1.0f;

				VkPipelineColorBlendStateCreateInfo cb;
				cb.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
				cb.flags = 0;
				cb.pNext = NULL;
				VkPipelineColorBlendAttachmentState att_state[1];
				att_state[0].colorWriteMask = 0xf;
				att_state[0].blendEnable = VK_FALSE;
				att_state[0].alphaBlendOp = VK_BLEND_OP_ADD;
				att_state[0].colorBlendOp = VK_BLEND_OP_ADD;
				att_state[0].srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;
				att_state[0].dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
				att_state[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
				att_state[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
				cb.attachmentCount = 1;
				cb.pAttachments = att_state;
				cb.logicOpEnable = VK_FALSE;
				cb.logicOp = VK_LOGIC_OP_NO_OP;
				cb.blendConstants[0] = 1.0f;
				cb.blendConstants[1] = 1.0f;
				cb.blendConstants[2] = 1.0f;
				cb.blendConstants[3] = 1.0f;

				VkPipelineViewportStateCreateInfo vp = {};
				vp.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
				vp.pNext = NULL;
				vp.flags = 0;
#ifndef __ANDROID__
				vp.viewportCount = 1;
				dynamicStateEnables[dynamicState.dynamicStateCount++] = VK_DYNAMIC_STATE_VIEWPORT;
				vp.scissorCount = 1;
				dynamicStateEnables[dynamicState.dynamicStateCount++] = VK_DYNAMIC_STATE_SCISSOR;
				vp.pScissors = NULL;
				vp.pViewports = NULL;
#else
				// Temporary disabling dynamic viewport on Android because some of drivers doesn't
    // support the feature.
    VkViewport viewports;
    viewports.minDepth = 0.0f;
    viewports.maxDepth = 1.0f;
    viewports.x = 0;
    viewports.y = 0;
    viewports.width = info.width;
    viewports.height = info.height;
    VkRect2D scissor;
    scissor.extent.width = info.width;
    scissor.extent.height = info.height;
    scissor.offset.x = 0;
    scissor.offset.y = 0;
    vp.viewportCount = NUM_VIEWPORTS;
    vp.scissorCount = NUM_SCISSORS;
    vp.pScissors = &scissor;
    vp.pViewports = &viewports;
#endif
				VkPipelineDepthStencilStateCreateInfo ds;
				ds.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
				ds.pNext = NULL;
				ds.flags = 0;
				ds.depthTestEnable = include_depth;
				ds.depthWriteEnable = include_depth;
				ds.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
				ds.depthBoundsTestEnable = VK_FALSE;
				ds.stencilTestEnable = VK_FALSE;
				ds.back.failOp = VK_STENCIL_OP_KEEP;
				ds.back.passOp = VK_STENCIL_OP_KEEP;
				ds.back.compareOp = VK_COMPARE_OP_ALWAYS;
				ds.back.compareMask = 0;
				ds.back.reference = 0;
				ds.back.depthFailOp = VK_STENCIL_OP_KEEP;
				ds.back.writeMask = 0;
				ds.minDepthBounds = 0;
				ds.maxDepthBounds = 0;
				ds.stencilTestEnable = VK_FALSE;
				ds.front = ds.back;

				VkPipelineMultisampleStateCreateInfo ms;
				ms.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
				ms.pNext = NULL;
				ms.flags = 0;
				ms.pSampleMask = NULL;
				ms.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
				ms.sampleShadingEnable = VK_FALSE;
				ms.alphaToCoverageEnable = VK_FALSE;
				ms.alphaToOneEnable = VK_FALSE;
				ms.minSampleShading = 0.0;

				VkGraphicsPipelineCreateInfo pipeline;
				pipeline.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
				pipeline.pNext = NULL;
				pipeline.layout = this->info.pipeline_layout;
				pipeline.basePipelineHandle = VK_NULL_HANDLE;
				pipeline.basePipelineIndex = 0;
				pipeline.flags = 0;
				pipeline.pVertexInputState = &vi;
				pipeline.pInputAssemblyState = &ia;
				pipeline.pRasterizationState = &rs;
				pipeline.pColorBlendState = &cb;
				pipeline.pTessellationState = NULL;
				pipeline.pMultisampleState = &ms;
				pipeline.pDynamicState = &dynamicState;
				pipeline.pViewportState = &vp;
				pipeline.pDepthStencilState = &ds;
				pipeline.pStages = this->info.shaderStages;
				pipeline.stageCount = 2;
				pipeline.renderPass = this->info.render_pass;
				pipeline.subpass = 0;

				res = vkCreateGraphicsPipelines(this->info.device, this->info.pipelineCache, 1, &pipeline, NULL, &this->info.pipeline);
				assert(res == VK_SUCCESS);
			}

		public:
			void bootstrapPipeline(VkBool32 depthPresent, bool use_texture)
			{
				this->initDescriptorPool(use_texture);
				this->initDescriptorSet(use_texture);
				this->initPipelineCache();
				this->initPipeline((VkBool32)depthPresent);
			}
		};

	}

}

#endif //OBSIDIAN2D_CORE_PIPELINE_H
