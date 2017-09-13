#ifndef _OBSIDIAN2D_CORE_WINDOWEVENT_
#define _OBSIDIAN2D_CORE_WINDOWEVENT_

#include "Obsidian2D/Util/Loggable.h"

namespace Obsidian2D
{
	namespace Core
	{
		enum WindowEvent
		{
			None       = 0,
			Click      = 1,
			Focus      = 2,
			Blur       = 3,
			Resize     = 4,
			Close      = 5,
			ButtonDown = 6,
			ButtonUp   = 7,
			Unknow     = 8,
		};
	}
}

#endif //_OBSIDIAN2D_CORE_WINDOWEVENT_
