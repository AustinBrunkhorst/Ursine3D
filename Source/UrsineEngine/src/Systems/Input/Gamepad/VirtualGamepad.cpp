#include "UrsinePrecompiled.h"

#include "VirtualGamepad.h"

#include "GamepadManager.h"

namespace Ursine
{
    VirtualGamepad::VirtualGamepad(void)
        : m_index(0) { }

    VirtualGamepad::VirtualGamepad(GamepadIndex index)
        : m_index( index ) { }

    bool VirtualGamepad::IsValid(void) const
    {
        return gGamepadManager->getVirtualGamepad( m_index ) != nullptr;
    }

    GamepadIndex VirtualGamepad::GetIndex(void) const
    {
        return GamepadManager::IndexToVirtual( m_index );
    }

    void VirtualGamepad::Press(GamepadButton button) const
    {
        auto gamepad = gGamepadManager->getVirtualGamepad( m_index );

        UAssert(gamepad != nullptr, "Virtual Gamepad is not valid");

        GamepadButtonArgs e = {
            button,
            GamepadManager::IndexToVirtual( m_index ),
            true,
            gamepad->state.IsButtonUp( button )
        };

        gamepad->state.virtualSetPressed( button );

        gGamepadManager->Dispatch( GP_BUTTON_DOWN, &e );
    }

    void VirtualGamepad::Release(GamepadButton button) const
    {
        auto gamepad = gGamepadManager->getVirtualGamepad( m_index );

        UAssert(gamepad != nullptr, "Virtual Gamepad is not valid");

        GamepadButtonArgs e = {
            button,
            GamepadManager::IndexToVirtual( m_index ),
            false,
            gamepad->state.IsButtonDown( button )
        };

        gamepad->state.virtualSetReleased( button );

        gGamepadManager->Dispatch( GP_BUTTON_UP, &e );
    }

    void VirtualGamepad::SetStickLeft(const Vec2 &left) const
    {
        auto gamepad = gGamepadManager->getVirtualGamepad( m_index );

        UAssert(gamepad != nullptr, "Virtual Gamepad is not valid");

        gamepad->state.m_sticksCurrent.setLeft( left );
    }

    void VirtualGamepad::SetStickRight(const Vec2 &right) const
    {
        auto gamepad = gGamepadManager->getVirtualGamepad( m_index );

        UAssert(gamepad != nullptr, "Virtual Gamepad is not valid");

        gamepad->state.m_sticksCurrent.setRight( right );
    }

    void VirtualGamepad::SetTriggerLeft(float left) const
    {
        auto gamepad = gGamepadManager->getVirtualGamepad( m_index );

        UAssert(gamepad != nullptr, "Virtual Gamepad is not valid");

        gamepad->state.m_triggersCurrent.setLeft( left );
    }

    void VirtualGamepad::SetTriggerRight(float right) const
    {
        auto gamepad = gGamepadManager->getVirtualGamepad( m_index );

        UAssert(gamepad != nullptr, "Virtual Gamepad is not valid");

        gamepad->state.m_triggersCurrent.setRight( right );
    }

    void VirtualGamepad::Remove(void)
    {
        gGamepadManager->removeVirtualGamepad( m_index );
    }
}
