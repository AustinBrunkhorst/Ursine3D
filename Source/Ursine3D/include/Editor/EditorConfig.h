/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EditorConfig.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

namespace ursine
{
	namespace ecs
	{
	#if defined(URSINE_WITH_EDITOR)

		extern void EditorClearDeletionQueue(void);

	#endif
	}
}
