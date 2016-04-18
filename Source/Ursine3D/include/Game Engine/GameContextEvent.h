/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** GameContextEvent.h
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
    enum GameContextEventType
    {
        GC_WINDOW_FOCUS_CHANGED
    };

    struct GameContextWindowFocusArgs : EventArgs
    {
        bool focused;

        GameContextWindowFocusArgs(bool focused)
            : focused( focused ) { }
    };
}