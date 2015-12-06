/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** MouseManagerEvent.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "EventArgs.h"

#include "MouseButton.h"

#include "Vec2.h"

namespace ursine
{
    enum MouseManagerEvent
    {
        MM_BUTTON_DOWN,
        MM_BUTTON_UP,
        MM_MOVE,
        MM_SCROLL
    };

    struct MouseButtonArgs : EventArgs
    {
        // Determines the button state. true if down, false if up
        bool state;

        // Button in question
        MouseButton button;

        // Position of the mouse when this event occurs
        Vec2 position;

        // Number of times this button was clicked. 
        // (1 is single click, 2 is double, etc)
        uint8 clicks;

        MouseButtonArgs(bool state, MouseButton button, const Vec2 &position, uint8 clicks)
            : state( state )
            , button( button )
            , position( position )
            , clicks( clicks ) { }
    };

    struct MouseMoveArgs : EventArgs
    {
        // New position of the mouse
        Vec2 position;

        // Change in position of the mouse since 
        // last motion event or mouse update
        Vec2 positionDelta;

        MouseMoveArgs(const Vec2 &position, const Vec2 &positionDelta)
            : position( position )
            , positionDelta( positionDelta ) { }
    };

    struct MouseScrollArgs : EventArgs
    {
        // Scroll wheel horizontal and vertical change
        Vec2 delta;

        MouseScrollArgs(const Vec2 &delta)
            : delta( delta ) { }
    };
}