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