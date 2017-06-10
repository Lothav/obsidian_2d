//
// Created by luizorv on 6/9/17.
//

#ifndef OBSIDIAN2D_CORE_SHADERS_H
#define OBSIDIAN2D_CORE_SHADERS_H

#ifdef __ANDROID__
// Android specific include files.
#include <unordered_map>

// Header files.
#include <android_native_app_glue.h>
#include "shaderc/shaderc.hpp"
// Static variable that keeps ANativeWindow and asset manager instances.
static android_app *Android_application = nullptr;
#elif (defined(VK_USE_PLATFORM_IOS_MVK) || defined(VK_USE_PLATFORM_MACOS_MVK))
#	include <MoltenGLSLToSPIRVConverter/GLSLToSPIRVConverter.h>
#else
// @TODO include GLSLANG
#	include "../../../external/glslang/SPIRV/GlslangToSpv.h"
#endif

#include "Util.h"

namespace Obsidian2D
{
	namespace Renderer
	{
		class Shaders : public Util
		{
		private:


#ifndef __ANDROID__
			void init_resources(TBuiltInResource &Resources) {
				Resources.maxLights = 32;
				Resources.maxClipPlanes = 6;
				Resources.maxTextureUnits = 32;
				Resources.maxTextureCoords = 32;
				Resources.maxVertexAttribs = 64;
				Resources.maxVertexUniformComponents = 4096;
				Resources.maxVaryingFloats = 64;
				Resources.maxVertexTextureImageUnits = 32;
				Resources.maxCombinedTextureImageUnits = 80;
				Resources.maxTextureImageUnits = 32;
				Resources.maxFragmentUniformComponents = 4096;
				Resources.maxDrawBuffers = 32;
				Resources.maxVertexUniformVectors = 128;
				Resources.maxVaryingVectors = 8;
				Resources.maxFragmentUniformVectors = 16;
				Resources.maxVertexOutputVectors = 16;
				Resources.maxFragmentInputVectors = 15;
				Resources.minProgramTexelOffset = -8;
				Resources.maxProgramTexelOffset = 7;
				Resources.maxClipDistances = 8;
				Resources.maxComputeWorkGroupCountX = 65535;
				Resources.maxComputeWorkGroupCountY = 65535;
				Resources.maxComputeWorkGroupCountZ = 65535;
				Resources.maxComputeWorkGroupSizeX = 1024;
				Resources.maxComputeWorkGroupSizeY = 1024;
				Resources.maxComputeWorkGroupSizeZ = 64;
				Resources.maxComputeUniformComponents = 1024;
				Resources.maxComputeTextureImageUnits = 16;
				Resources.maxComputeImageUniforms = 8;
				Resources.maxComputeAtomicCounters = 8;
				Resources.maxComputeAtomicCounterBuffers = 1;
				Resources.maxVaryingComponents = 60;
				Resources.maxVertexOutputComponents = 64;
				Resources.maxGeometryInputComponents = 64;
				Resources.maxGeometryOutputComponents = 128;
				Resources.maxFragmentInputComponents = 128;
				Resources.maxImageUnits = 8;
				Resources.maxCombinedImageUnitsAndFragmentOutputs = 8;
				Resources.maxCombinedShaderOutputResources = 8;
				Resources.maxImageSamples = 0;
				Resources.maxVertexImageUniforms = 0;
				Resources.maxTessControlImageUniforms = 0;
				Resources.maxTessEvaluationImageUniforms = 0;
				Resources.maxGeometryImageUniforms = 0;
				Resources.maxFragmentImageUniforms = 8;
				Resources.maxCombinedImageUniforms = 8;
				Resources.maxGeometryTextureImageUnits = 16;
				Resources.maxGeometryOutputVertices = 256;
				Resources.maxGeometryTotalOutputComponents = 1024;
				Resources.maxGeometryUniformComponents = 1024;
				Resources.maxGeometryVaryingComponents = 64;
				Resources.maxTessControlInputComponents = 128;
				Resources.maxTessControlOutputComponents = 128;
				Resources.maxTessControlTextureImageUnits = 16;
				Resources.maxTessControlUniformComponents = 1024;
				Resources.maxTessControlTotalOutputComponents = 4096;
				Resources.maxTessEvaluationInputComponents = 128;
				Resources.maxTessEvaluationOutputComponents = 128;
				Resources.maxTessEvaluationTextureImageUnits = 16;
				Resources.maxTessEvaluationUniformComponents = 1024;
				Resources.maxTessPatchComponents = 120;
				Resources.maxPatchVertices = 32;
				Resources.maxTessGenLevel = 64;
				Resources.maxViewports = 16;
				Resources.maxVertexAtomicCounters = 0;
				Resources.maxTessControlAtomicCounters = 0;
				Resources.maxTessEvaluationAtomicCounters = 0;
				Resources.maxGeometryAtomicCounters = 0;
				Resources.maxFragmentAtomicCounters = 8;
				Resources.maxCombinedAtomicCounters = 8;
				Resources.maxAtomicCounterBindings = 1;
				Resources.maxVertexAtomicCounterBuffers = 0;
				Resources.maxTessControlAtomicCounterBuffers = 0;
				Resources.maxTessEvaluationAtomicCounterBuffers = 0;
				Resources.maxGeometryAtomicCounterBuffers = 0;
				Resources.maxFragmentAtomicCounterBuffers = 1;
				Resources.maxCombinedAtomicCounterBuffers = 1;
				Resources.maxAtomicCounterBufferSize = 16384;
				Resources.maxTransformFeedbackBuffers = 4;
				Resources.maxTransformFeedbackInterleavedComponents = 64;
				Resources.maxCullDistances = 8;
				Resources.maxCombinedClipAndCullDistances = 8;
				Resources.maxSamples = 4;
				Resources.limits.nonInductiveForLoops = 1;
				Resources.limits.whileLoops = 1;
				Resources.limits.doWhileLoops = 1;
				Resources.limits.generalUniformIndexing = 1;
				Resources.limits.generalAttributeMatrixVectorIndexing = 1;
				Resources.limits.generalVaryingIndexing = 1;
				Resources.limits.generalSamplerIndexing = 1;
				Resources.limits.generalVariableIndexing = 1;
				Resources.limits.generalConstantMatrixVectorIndexing = 1;
			}

			EShLanguage FindLanguage(const VkShaderStageFlagBits shader_type) {
				switch (shader_type) {
					case VK_SHADER_STAGE_VERTEX_BIT:
						return EShLangVertex;

					case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
						return EShLangTessControl;

					case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
						return EShLangTessEvaluation;

					case VK_SHADER_STAGE_GEOMETRY_BIT:
						return EShLangGeometry;

					case VK_SHADER_STAGE_FRAGMENT_BIT:
						return EShLangFragment;

					case VK_SHADER_STAGE_COMPUTE_BIT:
						return EShLangCompute;

					default:
						return EShLangVertex;
				}
			}
#endif
		protected:

			const char *initialVertShaderText =
					"#version 400\n"
							"#extension GL_ARB_separate_shader_objects : enable\n"
							"#extension GL_ARB_shading_language_420pack : enable\n"
							"layout (std140, binding = 0) uniform bufferVals {\n"
							"    mat4 mvp;\n"
							"} myBufferVals;\n"
							"layout (location = 0) in vec4 pos;\n"
							"layout (location = 1) in vec4 inColor;\n"
							"layout (location = 0) out vec4 outColor;\n"
							"void main() {\n"
							"   outColor = inColor;\n"
							"   gl_Position = myBufferVals.mvp * pos;\n"
							"}\n";

			const char *initialFragShaderText =
					"#version 400\n"
							"#extension GL_ARB_separate_shader_objects : enable\n"
							"#extension GL_ARB_shading_language_420pack : enable\n"
							"layout (location = 0) in vec4 color;\n"
							"layout (location = 0) out vec4 outColor;\n"
							"void main() {\n"
							"   outColor = color;\n"
							"}\n";


			bool GLSLtoSPV(const VkShaderStageFlagBits shader_type, const char *pshader, std::vector<unsigned int> &spirv) {
#ifndef __ANDROID__
				EShLanguage stage = FindLanguage(shader_type);
				glslang::TShader shader(stage);
				glslang::TProgram program;
				const char *shaderStrings[1];
				TBuiltInResource Resources;
				init_resources(Resources);

				// Enable SPIR-V and Vulkan rules when parsing GLSL
				EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules);

				shaderStrings[0] = pshader;
				shader.setStrings(shaderStrings, 1);

				if (!shader.parse(&Resources, 100, false, messages)) {
					puts(shader.getInfoLog());
					puts(shader.getInfoDebugLog());
					return false;  // something didn't work
				}

				program.addShader(&shader);

				//
				// Program-level processing...
				//

				if (!program.link(messages)) {
					puts(shader.getInfoLog());
					puts(shader.getInfoDebugLog());
					fflush(stdout);
					return false;
				}

				glslang::GlslangToSpv(*program.getIntermediate(stage), spirv);
#else
				// On Android, use shaderc instead.
    shaderc::Compiler compiler;
    shaderc::SpvCompilationResult module =
        compiler.CompileGlslToSpv(pshader, strlen(pshader), MapShadercType(shader_type), "shader");
    if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
        LOGE("Error: Id=%d, Msg=%s", module.GetCompilationStatus(), module.GetErrorMessage().c_str());
        return false;
    }
    spirv.assign(module.cbegin(), module.cend());
#endif
				return true;
			}

			void init_glslang() {
#ifndef __ANDROID__
				glslang::InitializeProcess();
#endif
			}

			void finalize_glslang() {
#ifndef __ANDROID__
				glslang::FinalizeProcess();
#endif
			}

		};
	}
}
#endif //OBSIDIAN2D_CORE_SHADERS_H
