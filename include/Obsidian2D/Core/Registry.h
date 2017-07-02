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
			std::map<std::string, T> items;

		public:
			void add(const std::string& name, T item)
			{
				if(this->items.count(name)) {
					this->remove(name);
				}

				this->items[name] = item;
			}

			void remove(const std::string& name)
			{
				if(this->items.count(name)) {
					//Check if pointer or force pointer in template and delete it
					this->items.erase(name);
				}
			}

			T get(const std::string& name)
			{
				if(this->items.count(name)) {
					return this->items[name]->second;
				}

				return nullptr;
			}

			T getFirst()
			{
				if(this->items.size()) {
					return this->items.begin()->second;
				}

				return nullptr;
			}
		};
	}
}

#endif //_OBSIDIAN2D_CORE_REGISTRY_
