#ifndef _OBSIDIAN2D_CORE_ENTITY_
#define _OBSIDIAN2D_CORE_ENTITY_

namespace Obsidian2D
{
	namespace Core
	{
		class Entity
		{
		protected:
			bool inited = false;

		public:

			//Should check base functions strategy for childs
			void baseInit()
			{
				if(!this->inited) {
					this->init();
					this->inited = true;
				}
			}

			bool isInited()
			{
				return this->inited;
			}

			virtual void init()
			{

			}

			virtual void onInput(Obsidian2D::Core::WindowEvent e)
			{

			}

			virtual void onUpdate()
			{

			}

			//This sould likely receive a struct with some complete application state info.
			virtual void onDestroy()
			{

			}
		};
	}
}

#endif //_OBSIDIAN2D_CORE_ENTITY_
