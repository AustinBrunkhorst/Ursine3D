/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GamepadManagerEvent.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "GamepadIndex.h"
#include "GamepadButton.h"
#include "GamepadState.h"

namespace Ursine
{
    enum GamepadManagerEvent
    {
        // Gamepad is connected on start or was just connected
        GP_CONNECTED,
        // Gamepad was disconnected
        GP_DISCONNECTED,
        // Gamepad was pressed
        GP_BUTTON_DOWN,
        // Gamepad was released
        GP_BUTTON_UP
    };

    struct GamepadConnectionArgs : public EventArgs
    {
        bool connected;
        GamepadIndex index;

        GamepadConnectionArgs(bool connected, GamepadIndex index)
            : connected(connected)
            , index(index) {}
    };

    struct GamepadButtonArgs : public EventArgs
    {
        // which button this event represents
        GamepadButton button;

        // which gamepad this event came from
        GamepadIndex gamepad;

        // true if pressed, false if released
        bool pressed;

        // true if triggered, false if not
        bool triggered;

        GamepadButtonArgs(GamepadButton button, GamepadIndex gamepad, bool pressed, bool triggered)
            : button(button)
            , gamepad(gamepad)
            , pressed(pressed)
            , triggered(triggered) {}
    };

    struct GamepadTriggerArgs : public EventArgs
    {
        // trigger value 0 - 1
        float value;

        GamepadTriggerArgs(float Value)
            : value(Value) {}
    };
}