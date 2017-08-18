#ifndef _APP_SCENES_CHARACTERSELECTION_
#define _APP_SCENES_CHARACTERSELECTION_

#include "Obsidian2D/Core/Scene.h"


namespace App {
	namespace Scenes {
		class CharacterSelection: public Obsidian2D::Core::Scene
		{
		private:
		protected:
		public:

			void onUpdate()
			{
				this->log("CharacterSelection update!");
			}
		};
	}
}

#endif //_APP_SCENES_CHARACTERSELECTION_
