#ifndef _APP_APP_
#define _APP_APP_

#include "Obsidian2D/Core/App.h"

#include "Obsidian2D/Util/RectangleShape.h"


namespace App {
	class App: public Obsidian2D::Core::App
	{
	private:
	protected:
	public:
		App::Config getConfig()
		{
			auto config = Obsidian2D::Core::App::Config();
			config.windowWidth  = 1200;
			config.windowHeight = 800;
			return config;
		}

		void init(Obsidian2D::Core::Registry<Obsidian2D::Core::Scene>* registry)
		{

		}

		void onInput(Obsidian2D::Core::WindowEvent e)
		{
			this->log("Someone did something somewhere!");
			if(e == Obsidian2D::Core::WindowEvent::Click) {
				this->log("Click");
			}
		}

		void onUpdate()
		{

		}

		void onExit()
		{
			this->log("Bye");
		}
	};
}

#endif //_APP_APP_
