#ifndef _APP_APP_
#define _APP_APP_

#include "Obsidian2D/Core/App.h"
#include "Obsidian2D/Core/Registry.h"

namespace App {
	class App: public Obsidian2D::Core::App
	{
	private:
	protected:
	public:
		Obsidian2D::Core::App::Config getConfig()
		{
			auto config = Obsidian2D::Core::App::Config();
			config.windowWidth  = 1200;
			config.windowHeight = 800;
			return config;
		}

		void init(Obsidian2D::Core::Registry<Obsidian2D::Core::Scene>* registry)
		{
			//Obsidian2D::Image square = Util::Shapes::Square(10, 10, Util::Color::White);
		}
	};
}

#endif //_APP_APP_
