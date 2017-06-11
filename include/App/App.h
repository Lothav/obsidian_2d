#ifndef _APP_APP_
#define _APP_APP_

#include "Obsidian2D/Core/App.h"

namespace App {
	class App: public Obsidian2D::Core::App
	{
	private:
	protected:
	public:
		App()
		{
			//Obsidian2D::Image square = Util::Shapes::Square(10, 10, Util::Color::White);
		}

		Obsidian2D::Core::App::Config getConfig()
		{
			auto config = Obsidian2D::Core::App::Config();
			config.windowWidth  = 1200;
			config.windowHeight = 800;
			return config;
		}
	};
}

#endif //_APP_APP_
