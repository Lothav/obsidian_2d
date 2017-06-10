#ifndef OBSIDIAN2D_RENDERER_XCBWINDOW_
#define OBSIDIAN2D_RENDERER_XCBWINDOW_

#include "Obsidian2D/Renderer/Window.h"
#include "Obsidian2D/Core/WindowEvent.h"

namespace Obsidian2D
{
	namespace Renderer
	{
		class XcbWindow: public Window
		{
		public:

			~XcbWindow(){
				vkDestroyPipeline(this->info.device, this->info.pipeline, NULL);
				vkDestroyPipelineCache(this->info.device, this->info.pipelineCache, NULL);
				vkDestroyDescriptorPool(this->info.device, this->info.desc_pool, NULL);

				// Destroy vertex buffer
				vkDestroyBuffer(this->info.device, this->info.vertex_buffer.buf, NULL);
				vkFreeMemory(this->info.device, this->info.vertex_buffer.mem, NULL);

				// Destroy frame buffer
				for (uint32_t i = 0; i < this->info.swapchainImageCount; i++) {
					vkDestroyFramebuffer(this->info.device, this->info.framebuffers[i], NULL);
				}
				free(this->info.framebuffers);

				// Destroy shaders
				vkDestroyShaderModule(this->info.device, this->info.shaderStages[0].module, NULL);
				vkDestroyShaderModule(this->info.device, this->info.shaderStages[1].module, NULL);

				// Destroy render pass
				vkDestroyRenderPass(this->info.device, info.render_pass, NULL);

				// Destroy descriptor and pipeline layouts
				for (int i = 0; i < 1; i++) vkDestroyDescriptorSetLayout(this->info.device, this->info.desc_layout[i], NULL);
				vkDestroyPipelineLayout(this->info.device, this->info.pipeline_layout, NULL);

				// Destroy uniform buffer
				vkDestroyBuffer(this->info.device, this->info.uniform_data.buf, NULL);
				vkFreeMemory(this->info.device, this->info.uniform_data.mem, NULL);

				// Destroy depth buffer
				vkDestroyImageView(this->info.device, this->info.depth.view, NULL);
				vkDestroyImage(this->info.device, this->info.depth.image, NULL);
				vkFreeMemory(this->info.device, this->info.depth.mem, NULL);

				// Destroy Swap Chain
				for (uint32_t i = 0; i < info.swapchainImageCount; i++) {
					vkDestroyImageView(info.device, info.buffers[i].view, NULL);
				}
				vkDestroySwapchainKHR(info.device, info.swap_chain, NULL);

				// Destroy Command Buffers
				VkCommandBuffer cmd_bufs[1] = {this->info.cmd};
				vkFreeCommandBuffers(this->info.device, this->info.cmd_pool, 1, cmd_bufs);
				vkDestroyCommandPool(this->info.device, this->info.cmd_pool, NULL);

				vkDestroyDevice(this->device, NULL);

				// Detroy Window
				vkDestroySurfaceKHR(this->info.inst, this->info.surface, NULL);
				xcb_destroy_window(this->info.connection, this->info.window);
				xcb_disconnect(this->info.connection);

				vkDestroyInstance(this->info.inst, NULL);
			}
		private:
			int32_t width;
			int32_t height;

			void setWindowSize();
			void setXCBConnection(); // for Linux users :V
			void createWindow();
			void initSwapChainExtension();
		public:
			XcbWindow(int32_t width, int32_t height){
				this->width = width;
				this->height = height;
			}
			void bootstrap();
			void destroyWindow();

			::Obsidian2D::Core::WindowEvent poolEvent()
			{
				xcb_generic_event_t* e = nullptr;
				while ((e = xcb_poll_for_event(this->info.connection))) {
					if ((e->response_type & ~0x80) == XCB_CLIENT_MESSAGE) {
						if((*(xcb_client_message_event_t*)e).data.data32[0] == (*this->info.atom_wm_delete_window).atom) {
							return ::Obsidian2D::Core::WindowEvent::Close;
						}
					} else if((e->response_type & ~0x80) == XCB_ENTER_NOTIFY) {
						return ::Obsidian2D::Core::WindowEvent::Focus;
					} else if((e->response_type & ~0x80) == XCB_LEAVE_NOTIFY) {
						return ::Obsidian2D::Core::WindowEvent::Blur;
					} else if((e->response_type & ~0x80) == XCB_BUTTON_PRESS) {
						return ::Obsidian2D::Core::WindowEvent::ButtonDown;
					} else {
						return ::Obsidian2D::Core::WindowEvent::Unknow;
					}
				}

				return ::Obsidian2D::Core::WindowEvent::None;
			}
		};
	}
}

#endif //OBSIDIAN2D_RENDERER_XCBWINDOW_
