#ifndef _APP_APP_
#define _APP_APP_

#include "Obsidian2D/Core/App.h"

#include "Obsidian2D/Util/RectangleShape.h"

#include "App/Scenes/Opening.h"
#include "App/Scenes/Menu.h"
#include "App/Scenes/CharacterSelection.h"

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

		void init(Obsidian2D::Core::Registry<Obsidian2D::Core::Scene*>* sceneRegistry)
		{
			this->log("init()");

			//Hack for init, should just set default someway
			sceneRegistry->add("00_opening",             new Scenes::Opening());
			sceneRegistry->add("01_menu",                new Scenes::Menu());
			sceneRegistry->add("02_character_selection", new Scenes::CharacterSelection());
		}
	};
}

#endif //_APP_APP_
