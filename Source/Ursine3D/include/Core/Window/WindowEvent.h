/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** WindowEvent.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "EventArgs.h"

namespace ursine
{
    enum WindowEventType
    {
        WINDOW_RESIZE,
        WINDOW_FOCUS_CHANGED
    };

    struct WindowResizeArgs : public EventArgs
    {
        int width;
        int height;

        WindowResizeArgs(int width, int height)
            : width( width )
            , height( height ) { }
    };

    struct WindowFocusArgs : public EventArgs
    {
        bool focused;

        WindowFocusArgs(bool focused)
            : focused( focused ) { }
    };
}