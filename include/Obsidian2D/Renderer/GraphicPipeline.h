//
// Created by luizorv on 9/7/17.
//

#ifndef OBSIDIAN2D_GRAPHICPIPELINE_H
#define OBSIDIAN2D_GRAPHICPIPELINE_H

#include "Util.h"

namespace Obsidian2D
{
	namespace Renderer
	{
		class GraphicPipeline: public Util {

		private:
			VkPipelineCache 						pPipelineCache;
			VkPipeline vkPipeline;


		public:

			GraphicPipeline() {}

			VkPipeline getPipeline()
			{
				return vkPipeline;
			}

			void create(VkDevice device, VkPipelineLayout pipeline_layout, VkRenderPass render_pass, VkPipelineShaderStageCreateInfo* shaderStages)
			{

				VkPipelineCache 						pPipelineCache;
				VkPipeline vkPipeline;
				VkVertexInputBindingDescription vi_binding;
				vi_binding.binding 										= 0;
				vi_binding.inputRate 									= VK_VERTEX_INPUT_RATE_VERTEX;
				vi_binding.stride 										= sizeof(Vertex);

				std::vector<VkVertexInputAttributeDescription> vi_attribs;
				vi_attribs.resize(3);

				vi_attribs[0].binding 									= 0;
				vi_attribs[0].location 									= 0;
				vi_attribs[0].format 									= VK_FORMAT_R32G32B32_SFLOAT;
				vi_attribs[0].offset 									= offsetof(Vertex, pos);

				vi_attribs[1].binding 									= 0;
				vi_attribs[1].location 									= 1;
				vi_attribs[1].format 									= VK_FORMAT_R32G32_SFLOAT;
				vi_attribs[1].offset 									= offsetof(Vertex, uv);

				vi_attribs[2].binding 									= 0;
				vi_attribs[2].location 									= 2;
				vi_attribs[2].format 									= VK_FORMAT_R32G32B32_SFLOAT;
				vi_attribs[2].offset 									= offsetof(Vertex, normal);

				VkPipelineCacheCreateInfo pipelineCache;
				pipelineCache.sType 									= VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
				pipelineCache.pNext 									= NULL;
				pipelineCache.initialDataSize 							= 0;
				pipelineCache.pInitialData 								= NULL;
				pipelineCache.flags 									= 0;
				VkResult res = vkCreatePipelineCache(device, &pipelineCache, NULL, &pPipelineCache);
				assert(res == VK_SUCCESS);


				VkDynamicState dynamicStateEnables[VK_DYNAMIC_STATE_RANGE_SIZE];
				VkPipelineDynamicStateCreateInfo dynamicState = {};
				memset(dynamicStateEnables, 0, sizeof dynamicStateEnables);

				dynamicState.sType 										= VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
				dynamicState.pNext 										= NULL;
				dynamicState.pDynamicStates 							= dynamicStateEnables;
				dynamicState.dynamicStateCount 							= 0;

				VkPipelineVertexInputStateCreateInfo vi;
				memset(&vi, 0, sizeof(vi));
				vi.sType 												= VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
				vi.pNext 												= NULL;
				vi.flags 												= 0;
				vi.vertexBindingDescriptionCount 						= 1;
				vi.pVertexBindingDescriptions 							= &vi_binding;
				vi.vertexAttributeDescriptionCount 						= 3;
				vi.pVertexAttributeDescriptions 						= vi_attribs.data();

				VkPipelineInputAssemblyStateCreateInfo ia;
				ia.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
				ia.pNext = NULL;
				ia.flags = 0;
				ia.primitiveRestartEnable = VK_FALSE;
				ia.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

				VkPipelineRasterizationStateCreateInfo rs;
				rs.sType 												= VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
				rs.pNext 												= NULL;
				rs.flags 												= 0;
				rs.polygonMode 											= VK_POLYGON_MODE_FILL;
				rs.cullMode 											= VK_CULL_MODE_BACK_BIT;
				rs.frontFace 											= VK_FRONT_FACE_CLOCKWISE;
				rs.depthClampEnable 									= VK_FALSE;
				rs.rasterizerDiscardEnable 								= VK_FALSE;
				rs.depthBiasEnable 										= VK_FALSE;
				rs.depthBiasConstantFactor 								= 0;
				rs.depthBiasClamp 										= 0;
				rs.depthBiasSlopeFactor 								= 0;
				rs.lineWidth 											= 1.0f;

				VkPipelineColorBlendStateCreateInfo cb;
				cb.sType 												= VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
				cb.flags 												= 0;
				cb.pNext 												= NULL;

				VkPipelineColorBlendAttachmentState att_state[1];
				att_state[0].colorWriteMask 							= 0xf;
				att_state[0].blendEnable 								= VK_FALSE;
				att_state[0].alphaBlendOp 								= VK_BLEND_OP_ADD;
				att_state[0].colorBlendOp 								= VK_BLEND_OP_ADD;
				att_state[0].srcColorBlendFactor 						= VK_BLEND_FACTOR_ZERO;
				att_state[0].dstColorBlendFactor 						= VK_BLEND_FACTOR_ZERO;
				att_state[0].srcAlphaBlendFactor 						= VK_BLEND_FACTOR_ZERO;
				att_state[0].dstAlphaBlendFactor 						= VK_BLEND_FACTOR_ZERO;
				cb.attachmentCount 										= 1;
				cb.pAttachments 										= att_state;
				cb.logicOpEnable 										= VK_FALSE;
				cb.logicOp 												= VK_LOGIC_OP_NO_OP;
				cb.blendConstants[0] 									= 1.0f;
				cb.blendConstants[1] 									= 1.0f;
				cb.blendConstants[2] 									= 1.0f;
				cb.blendConstants[3] 									= 1.0f;

				VkPipelineViewportStateCreateInfo vp = {};
				vp.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
				vp.pNext = NULL;
				vp.flags = 0;
				vp.viewportCount 										= 1;
				dynamicStateEnables[dynamicState.dynamicStateCount++] 	= VK_DYNAMIC_STATE_VIEWPORT;
				vp.scissorCount 										= 1;
				dynamicStateEnables[dynamicState.dynamicStateCount++] 	= VK_DYNAMIC_STATE_SCISSOR;
				vp.pScissors 											= NULL;
				vp.pViewports 											= NULL;

				VkPipelineDepthStencilStateCreateInfo ds;
				ds.sType 												= VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
				ds.pNext 												= NULL;
				ds.flags 												= 0;
				ds.depthTestEnable 										= (VkBool32) true;
				ds.depthWriteEnable 									= (VkBool32) true;
				ds.depthCompareOp 										= VK_COMPARE_OP_LESS_OR_EQUAL;
				ds.depthBoundsTestEnable 								= VK_FALSE;
				ds.stencilTestEnable 									= VK_FALSE;
				ds.back.failOp 											= VK_STENCIL_OP_KEEP;
				ds.back.passOp 											= VK_STENCIL_OP_KEEP;
				ds.back.compareOp 										= VK_COMPARE_OP_ALWAYS;
				ds.back.compareMask 									= 0;
				ds.back.reference 										= 0;
				ds.back.depthFailOp 									= VK_STENCIL_OP_KEEP;
				ds.back.writeMask 										= 0;
				ds.minDepthBounds 										= 0;
				ds.maxDepthBounds 										= 0;
				ds.stencilTestEnable 									= VK_FALSE;
				ds.front 												= ds.back;

				VkPipelineMultisampleStateCreateInfo ms;
				ms.sType 												= VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
				ms.pNext 												= NULL;
				ms.flags 												= 0;
				ms.pSampleMask 											= NULL;
				ms.rasterizationSamples 								= VK_SAMPLE_COUNT_1_BIT;
				ms.sampleShadingEnable 									= VK_FALSE;
				ms.alphaToCoverageEnable 								= VK_FALSE;
				ms.alphaToOneEnable 									= VK_FALSE;
				ms.minSampleShading 									= 0.0;

				VkGraphicsPipelineCreateInfo pipeline;
				pipeline.sType 											= VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
				pipeline.pNext 											= NULL;
				pipeline.layout 										= pipeline_layout;
				pipeline.basePipelineHandle 							= VK_NULL_HANDLE;
				pipeline.basePipelineIndex 								= 0;
				pipeline.flags 											= 0;
				pipeline.pVertexInputState 								= &vi;
				pipeline.pInputAssemblyState 							= &ia;
				pipeline.pRasterizationState 							= &rs;
				pipeline.pColorBlendState 								= &cb;
				pipeline.pTessellationState 							= NULL;
				pipeline.pMultisampleState 								= &ms;
				pipeline.pDynamicState 									= &dynamicState;
				pipeline.pViewportState 								= &vp;
				pipeline.pDepthStencilState 							= &ds;
				pipeline.pStages 										= shaderStages;
				pipeline.stageCount 									= 2;
				pipeline.renderPass 									= render_pass;
				pipeline.subpass 										= 0;

				res = vkCreateGraphicsPipelines(device, pPipelineCache, 1, &pipeline, NULL, &vkPipeline);
				assert(res == VK_SUCCESS);
			}
		};
	}
}


#endif //OBSIDIAN2D_GRAPHICPIPELINE_H
