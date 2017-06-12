#ifndef _OBSIDIAN2D_CORE_APP_
#define _OBSIDIAN2D_CORE_APP_

#include "Obsidian2D/Util/Loggable.h"
#include "Obsidian2D/Core/Registry.h"

#include "Obsidian2D/Core/Scene.h"

namespace Obsidian2D
{
	namespace Core
	{
		class App : public Obsidian2D::Util::Loggable
		{
		public:
			//TODO: Check if i could do this in an App namespace or something better, like another file
			struct Config
			{
				std::string renderer = "default";
				bool debug = true;

				unsigned int windowWidth = 800;
				unsigned int windowHeight = 600;
			};

		private:

		protected:

		public:
			App()
			{

			}

			virtual void init(Obsidian2D::Core::Registry<Obsidian2D::Core::Scene>* registry)
			{
				this->log("base init()");
			}

			virtual Obsidian2D::Core::App::Config getConfig()
			{
				return Config();
			}
		};
	}
}

#endif //_OBSIDIAN2D_CORE_APP_
