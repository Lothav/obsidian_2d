#ifndef _APP_APP_
#define _APP_APP_

#include "Obsidian2D/Core/App.h"

#include "Obsidian2D/Util/RectangleShape.h"


namespace App {
	class App: public Obsidian2D::Core::App
	{
	private:
	protected:
		Obsidian2D::Util::RectangleShape rect;

	public:
		App::Config getConfig()
		{
			auto config = Obsidian2D::Core::App::Config();
			config.windowWidth  = 1200;
			config.windowHeight = 800;
			return config;
		}

		//Scenes should be registered here, and something like a resource pre-loading (Needs multithreading, pthread on posix?)
		void init(Obsidian2D::Core::Registry<Obsidian2D::Core::Scene>* registry)
		{
			this->rect.setPosition({10.0f, 10.0f});
		}

		//Input has to consider source, in case of multiplayer
		void onInput(Obsidian2D::Core::WindowEvent e)
		{
			if(e == Obsidian2D::Core::WindowEvent::Click) {
				this->log("Click");
				this->rect.move({1.0f, 0.0f});
			}
		}

		//onUpdate has to be accumulator aware for physics, maybe even two separate callbacks
		void onUpdate()
		{
			this->log("Rect horizontal pos: " + std::to_string(this->rect.getPosition().x));
		}

		//This sould likely receive a struct with some complete application state info.
		void onExit()
		{
			this->log("Bye");
		}
	};
}

#endif //_APP_APP_
