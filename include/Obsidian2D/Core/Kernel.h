#ifndef _OBSIDIAN2D_CORE_KERNEL_
#define _OBSIDIAN2D_CORE_KERNEL_
	#define OBSIDIAN2D_VERSION 0.0.1

	#ifdef __linux__
		#define LIKELY(x)   __builtin_expect((x),1)
		#define UNLIKELY(x) __builtin_expect((x),0)
	#elif _WIN32
		#define LIKELY(x)   x
		#define UNLIKELY(x) x
	#endif

#include "Obsidian2D/Util/Time.h"

#include "Obsidian2D/Util/Logger.h"
#include "Obsidian2D/Util/Loggable.h"

#include "Obsidian2D/Core/Error.h"

#include "Obsidian2D/Core/Registry.h"
#include "Obsidian2D/Core/App.h"

#include "Obsidian2D/Core/Scene.h"

#include "Obsidian2D/Renderer/XcbWindow.h"

namespace Obsidian2D
{
	namespace Core
	{
		class Kernel : public Obsidian2D::Util::Loggable
		{
		private:

		protected:

		public:
			Kernel()
			{
				this->registerLogCallback([](const std::string& line) {
					Obsidian2D::Util::Logger::write("[Obsidian2D::Core::Kernel] " + line);
				});
			}

			int start(Obsidian2D::Core::App* app)
			{
				this->log("start()");

				app->registerLogCallback([](const std::string& line) {
					Obsidian2D::Util::Logger::write("[Obsidian2D::Core::App] " + line);
				});


				Obsidian2D::Core::App::Config config = app->getConfig();

				//TODO: Create renderer base class and render blank screen
				Obsidian2D::Renderer::XcbWindow* xcbWindow = new Obsidian2D::Renderer::XcbWindow(config.windowWidth, config.windowHeight);

				/* Register routes, configs, default scene */
				app->baseInit();

				/* Register GameStates/Scenes */


				xcbWindow->registerLogCallback([](const std::string& line) {
					Obsidian2D::Util::Logger::write("[Obsidian2D::Renderer::Window] " + line);
				});

				if(config.debug) {
					//xcbWindow->logExtensions();
				}


				xcbWindow->bootstrap();

				/* Init defualt scene, register objects*/

				/* Game Loop */
				bool running = true;
				while(running) {
					while(WindowEvent e = xcbWindow->poolEvent()) {
						if(e == WindowEvent::Close) {
							this->log("[Window Event] Close");
							running = false;
						} else {
							//Send input to userspace
							app->baseOnInput(e);
						}
					}

					app->baseOnUpdate();
				}

				//Tell userspace we are exiting, do what you gotta do
				app->baseOnDestroy();

				this->log("Cleaning up memory");

				delete app;
				app = nullptr;

				delete xcbWindow;
				xcbWindow = nullptr;

				return Obsidian2D::Error::None;
			}
		};
	}
}

#endif //_OBSIDIAN2D_CORE_KERNEL_
