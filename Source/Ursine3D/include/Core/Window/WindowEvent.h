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