/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GamepadState.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "GamepadState.h"

namespace ursine
{
    GamepadState::GamepadState(void)
    {
        // Visual Studio doesn't like member initialization for static arrays
        memset( m_btnsLast, SDL_FALSE, BTN_MAX );
        memset( m_btnsCurrent, SDL_FALSE, BTN_MAX );
    }

    bool GamepadState::IsButtonDown(GamepadButton button, float threshold) const
    {
        switch (button)
        {
            case BTN_TRIGGER_LEFT:
                return m_triggersCurrent._left >= threshold;
            case BTN_TRIGGER_RIGHT:
                return m_triggersCurrent._right >= threshold;

            case BTN_STICK_LEFT_UP:
                return m_sticksCurrent.m_left.Y() >= threshold;
            case BTN_STICK_LEFT_DOWN:
                return m_sticksCurrent.m_left.Y() <= -threshold;
            case BTN_STICK_LEFT_LEFT:
                return m_sticksCurrent.m_left.X() <= -threshold;
            case BTN_STICK_LEFT_RIGHT:
                return m_sticksCurrent.m_left.X() >= threshold;

            case BTN_STICK_RIGHT_UP:
                return m_sticksCurrent.m_right.Y() >= threshold;
            case BTN_STICK_RIGHT_DOWN:
                return m_sticksCurrent.m_right.Y() <= -threshold;
            case BTN_STICK_RIGHT_LEFT:
                return m_sticksCurrent.m_right.X() <= -threshold;
            case BTN_STICK_RIGHT_RIGHT:
                return m_sticksCurrent.m_right.X() >= threshold;

            default:
                return m_btnsCurrent[ button ] == SDL_TRUE;
        }
    }

    bool GamepadState::IsButtonUp(GamepadButton button, float threshold) const
    {
        switch (button)
        {
            case BTN_TRIGGER_LEFT:
                return m_triggersCurrent._left < threshold;
            case BTN_TRIGGER_RIGHT:
                return m_triggersCurrent._right < threshold;

            case BTN_STICK_LEFT_UP:
                return m_sticksCurrent.m_left.Y() < threshold;
            case BTN_STICK_LEFT_DOWN:
                return m_sticksCurrent.m_left.Y() > -threshold;
            case BTN_STICK_LEFT_LEFT:
                return m_sticksCurrent.m_left.X() > -threshold;
            case BTN_STICK_LEFT_RIGHT:
                return m_sticksCurrent.m_left.X() < threshold;

            case BTN_STICK_RIGHT_UP:
                return m_sticksCurrent.m_right.Y() < threshold;
            case BTN_STICK_RIGHT_DOWN:
                return m_sticksCurrent.m_right.Y() > -threshold;
            case BTN_STICK_RIGHT_LEFT:
                return m_sticksCurrent.m_right.X() > -threshold;
            case BTN_STICK_RIGHT_RIGHT:
                return m_sticksCurrent.m_right.X() < threshold;

            default:
                return m_btnsCurrent[ button ] == SDL_FALSE;
        }
    }

    bool GamepadState::IsButtonTriggeredDown(GamepadButton button, float threshold) const
    {
        switch (button)
        {
            case BTN_TRIGGER_LEFT:
                return m_triggersLast._left < threshold &&
                    m_triggersCurrent._left >= threshold;
            case BTN_TRIGGER_RIGHT:
                return m_triggersLast._right < threshold &&
                    m_triggersCurrent._right >= threshold;

            case BTN_STICK_LEFT_UP:
                return m_sticksLast.m_left.Y() < threshold &&
                    m_sticksCurrent.m_left.Y() >= threshold;
            case BTN_STICK_LEFT_DOWN:
                return m_sticksLast.m_left.Y() > -threshold &&
                    m_sticksCurrent.m_left.Y() <= -threshold;
            case BTN_STICK_LEFT_LEFT:
                return m_sticksLast.m_left.X() > -threshold &&
                    m_sticksCurrent.m_left.X() <= -threshold;
            case BTN_STICK_LEFT_RIGHT:
                return m_sticksLast.m_left.X() < threshold &&
                    m_sticksCurrent.m_left.X() >= threshold;

            case BTN_STICK_RIGHT_UP:
                return m_sticksLast.m_right.Y() < threshold &&
                    m_sticksCurrent.m_right.Y() >= threshold;
            case BTN_STICK_RIGHT_DOWN:
                return m_sticksLast.m_right.Y() > -threshold &&
                    m_sticksCurrent.m_right.Y() <= -threshold;
            case BTN_STICK_RIGHT_LEFT:
                return m_sticksLast.m_right.X() > -threshold &&
                    m_sticksCurrent.m_right.X() <= -threshold;
            case BTN_STICK_RIGHT_RIGHT:
                return m_sticksLast.m_right.X() < threshold &&
                    m_sticksCurrent.m_right.X() >= threshold;

            default:
                return m_btnsLast[ button ] == SDL_FALSE &&
                    m_btnsCurrent[ button ] == SDL_TRUE;
        }
    }

    bool GamepadState::IsButtonTriggeredUp(GamepadButton button, float threshold) const
    {
        switch (button)
        {
            case BTN_TRIGGER_LEFT:
                return m_triggersLast._left >= threshold &&
                    m_triggersCurrent._left < threshold;
            case BTN_TRIGGER_RIGHT:
                return m_triggersLast._right >= threshold &&
                    m_triggersCurrent._right < threshold;

            case BTN_STICK_LEFT_UP:
                return m_sticksLast.m_left.Y() >= threshold &&
                    m_sticksCurrent.m_left.Y() < threshold;
            case BTN_STICK_LEFT_DOWN:
                return m_sticksLast.m_left.Y() <= -threshold &&
                    m_sticksCurrent.m_left.Y() > -threshold;
            case BTN_STICK_LEFT_LEFT:
                return m_sticksLast.m_left.X() <= -threshold &&
                    m_sticksCurrent.m_left.X() > -threshold;
            case BTN_STICK_LEFT_RIGHT:
                return m_sticksLast.m_left.X() >= threshold &&
                    m_sticksCurrent.m_left.X() < threshold;

            case BTN_STICK_RIGHT_UP:
                return m_sticksLast.m_right.Y() >= threshold &&
                    m_sticksCurrent.m_right.Y() < threshold;
            case BTN_STICK_RIGHT_DOWN:
                return m_sticksLast.m_right.Y() <= -threshold &&
                    m_sticksCurrent.m_right.Y() > -threshold;
            case BTN_STICK_RIGHT_LEFT:
                return m_sticksLast.m_right.X() <= -threshold &&
                    m_sticksCurrent.m_right.X() > -threshold;
            case BTN_STICK_RIGHT_RIGHT:
                return m_sticksLast.m_right.X() >= threshold &&
                    m_sticksCurrent.m_right.X() < threshold;

            default:
                return m_btnsLast[ button ] == SDL_TRUE &&
                    m_btnsCurrent[ button ] == SDL_FALSE;
        }
    }

    const GamepadThumbSticks &GamepadState::Sticks(void) const
    {
        return m_sticksCurrent;
    }

    const GamepadTriggers &GamepadState::Triggers(void) const
    {
        return m_triggersCurrent;
    }

    void GamepadState::applyCurrentToLast(void)
    {
        // copy buttons
        memcpy( m_btnsLast, m_btnsCurrent, BTN_MAX );

        m_sticksLast.m_left = m_sticksCurrent.m_left;
        m_sticksLast.m_right = m_sticksCurrent.m_right;

        m_triggersLast._left = m_triggersCurrent._left;
        m_triggersLast._right = m_triggersCurrent._right;
    }

    void GamepadState::virtualSetPressed(GamepadButton button)
    {
        UAssert(button < BTN_MAX, "Cannot set button state for virtual buttons (digital).");

        m_btnsCurrent[ button ] = SDL_TRUE;
    }

    void GamepadState::virtualSetReleased(GamepadButton button)
    {
        UAssert(button < BTN_MAX, "Cannot set button state for virtual buttons (digital).");

        m_btnsCurrent[ button ] = SDL_FALSE;
    }
}
