#pragma once

#include "EventArgs.h"

#include "MouseButton.h"

#include "Vector2.h"

namespace Ursine
{
    enum MouseManagerEvent
    {
        MM_BUTTON_DOWN,
        MM_BUTTON_UP,
        MM_MOVE,
        MM_SCROLL
    };

    struct MouseButtonArgs : public EventArgs
    {
        // Determines the button state. true if down, false if up
        bool state;

        // Button in question
        MouseButton button;

        // Position of the mouse when this event occurs
        Vector2 position;

        // Number of times this button was clicked. 
        // (1 is single click, 2 is double, etc)
        uint8 clicks;

        MouseButtonArgs(bool state, MouseButton button, const Vector2 &position, uint8 clicks)
            : state(state)
            , button(button)
            , position(position)
            , clicks(clicks) { }
    };

    struct MouseMoveArgs : public EventArgs
    {
        // New position of the mouse
        Vector2 position;

        // Change in position of the mouse since 
        // last motion event or mouse update
        Vector2 position_delta;

        MouseMoveArgs(const Vector2 &position, const Vector2 &position_delta)
            : position(position)
            , position_delta(position_delta) { }
    };

    struct MouseScrollArgs : public EventArgs
    {
        // Scroll wheel horizontal and vertical change
        Vector2 delta;

        MouseScrollArgs(const Vector2 &delta)
            : delta(delta) { }
    };
}