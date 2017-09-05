//
// Created by tracksale on 9/5/17.
//

#ifndef OBSIDIAN2D_GRAPHICPIPELINE_H
#define OBSIDIAN2D_GRAPHICPIPELINE_H

#include <Obsidian2D/Renderer/vulkan/vulkan.h>
#include <assert.h>
#include <vector>
#include <cstring>
#include "Util.h"


struct VertexA {
    float pos[3];
    float uv[2];
    float normal[3];
};


namespace Obsidian2D
{
    namespace Renderer
    {
        class GraphicPipeline {


        private:

            VkDevice                        _instance_device = nullptr;
            VkPipeline                      _pipeline = nullptr;
            VkGraphicsPipelineCreateInfo    _pipeline_ci;
            VkPipelineCache                 _pipeline_cache = nullptr;

        public:

            GraphicPipeline(VkDevice device)
            {
                _instance_device = device;
            }

            void create(VkPipelineLayout pipelineLayout, VkRenderPass renderPass)
            {
                _pipeline_ci.sType 													= VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
                _pipeline_ci.pNext 													= nullptr;
                _pipeline_ci.layout 												= pipelineLayout;
                _pipeline_ci.basePipelineHandle 									= VK_NULL_HANDLE;
                _pipeline_ci.basePipelineIndex 										= 0;
                _pipeline_ci.flags 													= 0;
                _pipeline_ci.pTessellationState 									= nullptr;
                _pipeline_ci.stageCount 											= 2;
                _pipeline_ci.renderPass 											= renderPass;
                _pipeline_ci.subpass 												= 0;

                setShaderStages();
                setPipelineCI();

                VkResult res = vkCreateGraphicsPipelines(_instance_device, _pipeline_cache, 1, &_pipeline_ci, nullptr, &_pipeline);
                assert(res == VK_SUCCESS);
            }

            VkPipeline getPipeline()
            {
                return _pipeline;
            }

        private:


            void setShaderStages()
            {

                VkPipelineShaderStageCreateInfo shaderStages[2];

                shaderStages[0].sType       = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                shaderStages[0].stage       = VK_SHADER_STAGE_VERTEX_BIT;
                shaderStages[0].module      = Util::loadSPIRVShader("shaders/triangle/vert.spv", _instance_device);
                shaderStages[0].pName       = "main";
                assert(shaderStages[0].module != VK_NULL_HANDLE);

                shaderStages[1].sType       = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                shaderStages[1].stage       = VK_SHADER_STAGE_FRAGMENT_BIT;
                shaderStages[1].module      = Util::loadSPIRVShader("shaders/triangle/frag.spv", _instance_device);
                shaderStages[1].pName       = "main";
                assert(shaderStages[1].module != VK_NULL_HANDLE);

                _pipeline_ci.pStages        = shaderStages;
            }

            void setPipelineCI()
            {

                VkVertexInputBindingDescription vi_binding;
                vi_binding.binding 										= 0;
                vi_binding.inputRate 									= VK_VERTEX_INPUT_RATE_VERTEX;
                vi_binding.stride 										= sizeof(VertexA);

                std::vector<VkVertexInputAttributeDescription> vi_attribs;
                vi_attribs.resize(3);

                vi_attribs[0].binding 									= 0;
                vi_attribs[0].location 									= 0;
                vi_attribs[0].format 									= VK_FORMAT_R32G32B32_SFLOAT;
                vi_attribs[0].offset 									= offsetof(VertexA, pos);

                vi_attribs[1].binding 									= 0;
                vi_attribs[1].location 									= 1;
                vi_attribs[1].format 									= VK_FORMAT_R32G32_SFLOAT;
                vi_attribs[1].offset 									= offsetof(VertexA, uv);

                vi_attribs[2].binding 									= 0;
                vi_attribs[2].location 									= 2;
                vi_attribs[2].format 									= VK_FORMAT_R32G32B32_SFLOAT;
                vi_attribs[2].offset 									= offsetof(VertexA, normal);

                VkPipelineCacheCreateInfo pipelineCache;
                pipelineCache.sType 									= VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
                pipelineCache.pNext 									= nullptr;
                pipelineCache.initialDataSize 							= 0;
                pipelineCache.pInitialData 								= nullptr;
                pipelineCache.flags 									= 0;
                VkResult res = vkCreatePipelineCache(_instance_device, &pipelineCache, nullptr, &_pipeline_cache);
                assert(res == VK_SUCCESS);


                VkDynamicState dynamicStateEnables[VK_DYNAMIC_STATE_RANGE_SIZE];
                VkPipelineDynamicStateCreateInfo dynamicState = {};
                memset(dynamicStateEnables, 0, sizeof dynamicStateEnables);

                dynamicState.sType 										= VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
                dynamicState.pNext 										= nullptr;
                dynamicState.pDynamicStates 							= dynamicStateEnables;
                dynamicState.dynamicStateCount 							= 0;
                _pipeline_ci.pDynamicState 								= &dynamicState;


                VkPipelineVertexInputStateCreateInfo vi;
                memset(&vi, 0, sizeof(vi));
                vi.sType 												= VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
                vi.pNext 											    = nullptr;
                vi.flags 											    = 0;
                vi.vertexBindingDescriptionCount 					    = 1;
                vi.pVertexBindingDescriptions 						    = &vi_binding;
                vi.vertexAttributeDescriptionCount 					    = 3;
                vi.pVertexAttributeDescriptions 					    = vi_attribs.data();
                _pipeline_ci.pVertexInputState 							= &vi;



                VkPipelineInputAssemblyStateCreateInfo ia;
                ia.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
                ia.pNext = nullptr;
                ia.flags = 0;
                ia.primitiveRestartEnable = VK_FALSE;
                ia.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
                _pipeline_ci.pInputAssemblyState 						= &ia;


                VkPipelineRasterizationStateCreateInfo rs;
                rs.sType 												= VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
                rs.pNext 												= nullptr;
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
                _pipeline_ci.pRasterizationState 						= &rs;


                VkPipelineColorBlendStateCreateInfo cb;
                cb.sType 												= VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
                cb.flags 												= 0;
                cb.pNext 												= nullptr;

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
                _pipeline_ci.pColorBlendState 							= &cb;


                VkPipelineViewportStateCreateInfo vp = {};
                vp.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
                vp.pNext = nullptr;
                vp.flags = 0;
                vp.viewportCount 										= 1;
                dynamicStateEnables[dynamicState.dynamicStateCount++] 	= VK_DYNAMIC_STATE_VIEWPORT;
                vp.scissorCount 										= 1;
                dynamicStateEnables[dynamicState.dynamicStateCount++] 	= VK_DYNAMIC_STATE_SCISSOR;
                vp.pScissors 											= nullptr;
                vp.pViewports 											= nullptr;
                _pipeline_ci.pViewportState 							= &vp;


                VkPipelineDepthStencilStateCreateInfo ds;
                ds.sType 												= VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
                ds.pNext 												= nullptr;
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
                _pipeline_ci.pDepthStencilState 						= &ds;

                VkPipelineMultisampleStateCreateInfo ms;
                ms.sType 												= VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
                ms.pNext 												= nullptr;
                ms.flags 												= 0;
                ms.pSampleMask 											= nullptr;
                ms.rasterizationSamples 								= VK_SAMPLE_COUNT_1_BIT;
                ms.sampleShadingEnable 									= VK_FALSE;
                ms.alphaToCoverageEnable 								= VK_FALSE;
                ms.alphaToOneEnable 									= VK_FALSE;
                ms.minSampleShading 									= 0.0;
                _pipeline_ci.pMultisampleState 							= &ms;
            }


        };
    }
}
#endif //OBSIDIAN2D_GRAPHICPIPELINE_H
