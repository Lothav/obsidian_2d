#ifndef _APP_SCENES_MENU_
#define _APP_SCENES_MENU_

#include "Obsidian2D/Core/Scene.h"


namespace App {
	namespace Scenes {
		class Menu: public Obsidian2D::Core::Scene
		{
		private:
		protected:
		public:

			void onUpdate()
			{
				this->log("Menu update!");
			}
		};
	}
}

#endif //_APP_SCENES_MENU_
