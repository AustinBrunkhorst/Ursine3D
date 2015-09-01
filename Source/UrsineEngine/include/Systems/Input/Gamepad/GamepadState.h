/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GamepadState.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "GamepadThumbSticks.h"
#include "GamepadTriggers.h"
#include "GamepadButton.h"

namespace ursine
{
    class GamepadState
    {
        friend class GamepadManager;
        friend class VirtualGamepad;

        uint8 m_btnsLast[BTN_MAX];
        uint8 m_btnsCurrent[BTN_MAX];

        GamepadThumbSticks m_sticksLast;
        GamepadThumbSticks m_sticksCurrent;

        GamepadTriggers m_triggersLast;
        GamepadTriggers m_triggersCurrent;

        // sets the current state to the last
        void applyCurrentToLast(void);

        void virtualSetPressed(GamepadButton button);
        void virtualSetReleased(GamepadButton button);
    public:
        GamepadState(void);

        // Determines if a button is pressed for this state
        bool IsButtonDown(GamepadButton button, float threshold = 0.5f) const;

        // Determines if a button is released for this state
        bool IsButtonUp(GamepadButton button, float threshold = 0.5f) const;

        // Determines if the button is triggered (down)
        bool IsButtonTriggeredDown(GamepadButton button, float threshold = 0.5f) const;

        // Determines if the button is triggered (up)
        bool IsButtonTriggeredUp(GamepadButton button, float threshold = 0.5f) const;

        // Gets the current thumbsticks state
        const GamepadThumbSticks &Sticks(void) const;

        // Gets the current triggers state
        const GamepadTriggers &Triggers(void) const;
    };
}
