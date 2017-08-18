#ifndef _OBSIDIAN2D_CORE_MOVABLE_
#define _OBSIDIAN2D_CORE_MOVABLE_

#include "Obsidian2D/Core/Vec2f.h"

namespace Obsidian2D
{
	namespace Core
	{
		class Movable
		{
		private:
		protected:
			Vec2f position;

		public:
			Movable()
			{

			}

			void setPosition(const Vec2f& newPosition)
			{
				this->position.x = newPosition.x;
				this->position.y = newPosition.y;
			}

			const Vec2f& getPosition()
			{
				return this->position;
			}

			void move(const Vec2f& deltaPosition)
			{
				this->position.x += deltaPosition.x;
				this->position.y += deltaPosition.y;
			}
		};
	}
}

#endif //_OBSIDIAN2D_CORE_MOVABLE_
