#ifndef _OBSIDIAN2D_CORE_VECTOR_
#define _OBSIDIAN2D_CORE_VECTOR_

namespace Obsidian2D
{
	namespace Core
	{
		struct Vec2f {
			float x = 0.0f;
			float y = 0.0f;

			Vec2f(float x = 0.0f, float y = 0.0f)
			{
				this->x = x;
				this->y = y;
			}
		};
	}
}
#endif //_OBSIDIAN2D_CORE_VECTOR_
