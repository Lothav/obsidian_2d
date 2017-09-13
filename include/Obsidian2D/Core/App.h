#ifndef _OBSIDIAN2D_CORE_APP_
#define _OBSIDIAN2D_CORE_APP_

#include "Obsidian2D/Util/Loggable.h"
#include "Obsidian2D/Core/Registry.h"
#include "Obsidian2D/Core/WindowEvent.h"

#include "Obsidian2D/Core/Scene.h"

namespace Obsidian2D
{
	namespace Core
	{
		class App : public Obsidian2D::Util::Loggable
		{
		public:
			//TODO: Check if i could do this in an App namespace or something better, like another file
			struct Config
			{
				std::string renderer = "default";
				bool debug = true;

				unsigned int windowWidth = 800;
				unsigned int windowHeight = 600;
			};

		private:

		protected:
			Obsidian2D::Core::Registry<Scene*>* sceneRegistry = nullptr;
			Scene* currentScene = nullptr;

		public:
			App()
			{
				this->sceneRegistry = new Obsidian2D::Core::Registry<Scene*>();
				this->sceneRegistry->registerLogCallback([](const std::string& line) {
					Obsidian2D::Util::Logger::write("[Obsidian2D::Core::Registry] " + line);
				});
			}

			void baseInit()
			{
				this->log("baseInit()");
				this->init(this->sceneRegistry);

				//std::cout << sceneRegistry->getFirst()// << std::endl;
				this->currentScene = sceneRegistry->getFirst();
				this->currentScene->init();

			}

			virtual void init(Obsidian2D::Core::Registry<Obsidian2D::Core::Scene*>* sceneRegistry)
			{

			}

			virtual Obsidian2D::Core::App::Config getConfig()
			{
				return Config();
			}

			void baseOnInput(Obsidian2D::Core::WindowEvent e)
			{
				this->onInput(e);
				this->currentScene->onInput(e);
			}

			virtual void onInput(Obsidian2D::Core::WindowEvent e)
			{

			}

			void baseOnUpdate()
			{
				this->onUpdate();
				this->currentScene->onUpdate();
			}

			virtual void onUpdate()
			{

			}

			void baseOnDestroy()
			{
				//Should destroy all
				this->currentScene->onDestroy();
			}

			virtual void onDestroy()
			{

			}

		};
	}
}

#endif //_OBSIDIAN2D_CORE_APP_
