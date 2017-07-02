#ifndef _APP_SCENES_OPENING_
#define _APP_SCENES_OPENING_

#include "Obsidian2D/Core/Scene.h"


namespace App {
	namespace Scenes {
		class Opening: public Obsidian2D::Core::Scene
		{
		private:
		protected:
		public:

			void onUpdate()
			{
				this->log("Opening update!");
			}
		};
	}
}

#endif //_APP_SCENES_OPENING_
