/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** VirtualGamepad.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "GamepadIndex.h"
#include "GamepadButton.h"

namespace ursine
{
    class GamepadManager;

    class VirtualGamepad
    {
        friend class GamepadManager;

        GamepadIndex m_index;
        GamepadManager *m_manager;

        VirtualGamepad(GamepadManager *manager, GamepadIndex index);
    public:
        VirtualGamepad(GamepadManager *manager);

        static const GamepadIndex INDEX_FLAG = 1u << 31u;

        // Determines if this gamepad is valid (exists in the gamepad manager)
        bool IsValid(void) const;

        // Gets the GamepadIndex for this virtual gamepad
        GamepadIndex GetIndex(void) const;

        void Press(GamepadButton button) const;
        void Release(GamepadButton button) const;

        void SetStickLeft(const Vec2 &left) const;
        void SetStickRight(const Vec2 &right) const;

        void SetTriggerLeft(float left) const;
        void SetTriggerRight(float right) const;

        // Removes this virtual gamepad from the gamepad manager
        void Remove(void);
    };
}