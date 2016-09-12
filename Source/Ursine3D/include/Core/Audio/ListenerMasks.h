/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ListenerMasks.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

namespace ursine
{
	enum class ListenerIndex
	{
        NONE = -1,
		L1,
		L2,
		L3,
		L4,
		L5,
		L6,
		L7,
		L8
    } Meta(Enable);

	enum class ListenerMask
    {
        NONE = 0,
        L1 = 1 << 0u,
        L2 = 1 << 1u,
        L3 = 1 << 2u,
        L4 = 1 << 3u,
        L5 = 1 << 4u,
        L6 = 1 << 5u,
        L7 = 1 << 6u,
        L8 = 1 << 7u
    } Meta(Enable, BitMaskEditor);
}
