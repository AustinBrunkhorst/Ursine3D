/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ScenePlayState.h
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
    enum ScenePlayState
    {
        PS_PLAYING,
        PS_PAUSED,

    #if defined(URSINE_WITH_EDITOR)
        PS_EDITOR
    #endif
    };
}