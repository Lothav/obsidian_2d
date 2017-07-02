#ifndef _OBSIDIAN2D_CORE_SCENE_
#define _OBSIDIAN2D_CORE_SCENE_

#include "Obsidian2D/Util/Loggable.h"
#include "Obsidian2D/Core/Entity.h"

namespace Obsidian2D {
	namespace Core {
		class Scene: public Obsidian2D::Util::Loggable, public Obsidian2D::Core::Entity
		{
		private:
		protected:
		public:
			Scene()
			{

			}
		};
	}
}

#endif //_OBSIDIAN2D_CORE_SCENE_
