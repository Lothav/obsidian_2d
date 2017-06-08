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
