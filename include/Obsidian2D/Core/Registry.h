#ifndef _OBSIDIAN2D_CORE_REGISTRY_
#define _OBSIDIAN2D_CORE_REGISTRY_

#include <map>

#include "Obsidian2D/Util/Loggable.h"

namespace Obsidian2D
{
	namespace Core
	{
		template<typename T>
		class Registry : public Obsidian2D::Util::Loggable
		{
		private:

		protected:
			std::map<std::string, T*> items;

		public:
			void add(const std::string& name, T* item)
			{
				//Check if exists and shit
				this->items[name] = item;
			}

			void remove()
			{
				//Remove and stuff
			}

			T* get(const std::string& name)
			{
				return this->items[name];
			}
		};
	}
}

#endif //_OBSIDIAN2D_CORE_REGISTRY_
