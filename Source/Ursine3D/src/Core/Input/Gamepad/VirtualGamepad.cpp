/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** VirtualGamepad.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "VirtualGamepad.h"

#include "GamepadManager.h"

namespace ursine
{
    VirtualGamepad::VirtualGamepad(GamepadManager *manager)
        : m_index( 0 )
        , m_manager( manager ) { }

    VirtualGamepad::VirtualGamepad(GamepadManager *manager, GamepadIndex index)
        : m_index( index )
        , m_manager( manager ) { }

    bool VirtualGamepad::IsValid(void) const
    {
        return m_manager->getVirtualGamepad( m_index ) != nullptr;
    }

    GamepadIndex VirtualGamepad::GetIndex(void) const
    {
        return GamepadManager::IndexToVirtual( m_index );
    }

    void VirtualGamepad::Press(GamepadButton button) const
    {
        auto gamepad = m_manager->getVirtualGamepad( m_index );

        UAssert(gamepad != nullptr, "Virtual Gamepad is not valid");

        GamepadButtonArgs e = {
            button,
            GamepadManager::IndexToVirtual( m_index ),
            true,
            gamepad->state.IsButtonUp( button )
        };

        gamepad->state.virtualSetPressed( button );

        m_manager->Dispatch( GP_BUTTON_DOWN, &e );
    }

    void VirtualGamepad::Release(GamepadButton button) const
    {
        auto gamepad = m_manager->getVirtualGamepad( m_index );

        UAssert(gamepad != nullptr, "Virtual Gamepad is not valid");

        GamepadButtonArgs e = {
            button,
            GamepadManager::IndexToVirtual( m_index ),
            false,
            gamepad->state.IsButtonDown( button )
        };

        gamepad->state.virtualSetReleased( button );

        m_manager->Dispatch( GP_BUTTON_UP, &e );
    }

    void VirtualGamepad::SetStickLeft(const Vec2 &left) const
    {
        auto gamepad = m_manager->getVirtualGamepad( m_index );

        UAssert(gamepad != nullptr, "Virtual Gamepad is not valid");

        gamepad->state.m_sticksCurrent.setLeft( left );
    }

    void VirtualGamepad::SetStickRight(const Vec2 &right) const
    {
        auto gamepad = m_manager->getVirtualGamepad( m_index );

        UAssert(gamepad != nullptr, "Virtual Gamepad is not valid");

        gamepad->state.m_sticksCurrent.setRight( right );
    }

    void VirtualGamepad::SetTriggerLeft(float left) const
    {
        auto gamepad = m_manager->getVirtualGamepad( m_index );

        UAssert(gamepad != nullptr, "Virtual Gamepad is not valid");

        gamepad->state.m_triggersCurrent.setLeft( left );
    }

    void VirtualGamepad::SetTriggerRight(float right) const
    {
        auto gamepad = m_manager->getVirtualGamepad( m_index );

        UAssert(gamepad != nullptr, "Virtual Gamepad is not valid");

        gamepad->state.m_triggersCurrent.setRight( right );
    }

    void VirtualGamepad::Remove(void)
    {
        m_manager->removeVirtualGamepad( m_index );
    }
}
