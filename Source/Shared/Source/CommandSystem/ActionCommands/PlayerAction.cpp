/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayerAction.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "PlayerAction.h"
#include <Components/PlayerInputComponent.h>
#include <Core/CoreSystem.h>
#include <Core/Input/Gamepad/GamepadManager.h>

PlayerAction::PlayerAction()
    : m_entity(nullptr)
    , m_actionMode(ActionCount)
    , m_binding(BindingCount)
{
    
}

PlayerAction::PlayerAction(ursine::ecs::Entity* entity, const ActionMode action, const InputBinding binding)
    : m_entity(entity)
    , m_actionMode(action)
    , m_binding(binding)
{
    
}

bool PlayerAction::WasPressed() const
{
    auto *input = GetCoreSystem(ursine::GamepadManager)->GetState(m_entity->GetComponent<PlayerInput>()->id);
    
    if ( input == nullptr )
        return false;
    
    switch(m_binding)
    {
    case Action1:
        return input->IsButtonTriggeredDown(ursine::GamepadButton::BTN_A);
    case Action2:
        return input->IsButtonTriggeredDown(ursine::GamepadButton::BTN_B);
    case Action3:
        return input->IsButtonTriggeredDown(ursine::GamepadButton::BTN_X);
    case RightTrigger:
        return input->IsButtonTriggeredDown(ursine::GamepadButton::BTN_TRIGGER_RIGHT);
    default:
        return input->IsButtonTriggeredDown(ursine::GamepadButton::BTN_Y);
    }
}

bool PlayerAction::IsPressed() const
{
    auto *input = GetCoreSystem(ursine::GamepadManager)->GetState(m_entity->GetComponent<PlayerInput>()->id);

    if ( input == nullptr )
        return false;

    switch ( m_binding )
    {
    case Action1:
        return input->IsButtonDown(ursine::GamepadButton::BTN_A);
    case Action2:
        return input->IsButtonDown(ursine::GamepadButton::BTN_B);
    case Action3:
        return input->IsButtonDown(ursine::GamepadButton::BTN_X);
    case RightTrigger:
        return input->IsButtonDown(ursine::GamepadButton::BTN_TRIGGER_RIGHT);
    default:
        return input->IsButtonDown(ursine::GamepadButton::BTN_Y);
    }
}

bool PlayerAction::WasReleased() const
{
    auto *input = GetCoreSystem(ursine::GamepadManager)->GetState(m_entity->GetComponent<PlayerInput>()->id);

    if ( input == nullptr )
        return false;

    switch ( m_binding )
    {
    case Action1:
        return input->IsButtonTriggeredUp(ursine::GamepadButton::BTN_A);
    case Action2:
        return input->IsButtonTriggeredUp(ursine::GamepadButton::BTN_B);
    case Action3:
        return input->IsButtonTriggeredUp(ursine::GamepadButton::BTN_X);
    case RightTrigger:
        return input->IsButtonTriggeredUp(ursine::GamepadButton::BTN_TRIGGER_RIGHT);
    default:
        return input->IsButtonTriggeredUp(ursine::GamepadButton::BTN_Y);
    }
}

bool PlayerAction::StickUp() const
{
    auto *input = GetCoreSystem(ursine::GamepadManager)->GetState(m_entity->GetComponent<PlayerInput>()->id);

    if ( input == nullptr )
        return false;

    switch ( m_binding )
    {
    case LeftStickUp:
        return input->Sticks().Left().Y() > 0;
    default:
        return input->Sticks().Right().Y() > 0;
    }
}

bool PlayerAction::StickDown() const
{
    auto *input = GetCoreSystem(ursine::GamepadManager)->GetState(m_entity->GetComponent<PlayerInput>()->id);

    if ( input == nullptr )
        return false;

    switch ( m_binding )
    {
    case LeftStickDown:
        return input->Sticks().Left().Y() < 0;
    default:
        return input->Sticks().Right().Y() < 0;
    }
}

bool PlayerAction::StickLeft() const
{
    auto *input = GetCoreSystem(ursine::GamepadManager)->GetState(m_entity->GetComponent<PlayerInput>()->id);

    if ( input == nullptr )
        return false;

    switch ( m_binding )
    {
    case LeftStickLeft:
        return input->Sticks().Left().Y() < 0;
    default:
        return input->Sticks().Right().Y() < 0;
    }
}

bool PlayerAction::StickRight() const
{
    auto *input = GetCoreSystem(ursine::GamepadManager)->GetState(m_entity->GetComponent<PlayerInput>()->id);

    if ( input == nullptr )
        return false;

    switch ( m_binding )
    {
    case LeftStickRight:
        return input->Sticks().Left().Y() > 0;
    default:
        return input->Sticks().Right().Y() > 0;
    }
}

ursine::Vec2 PlayerAction::GetAxis() const
{
    auto *input = GetCoreSystem(ursine::GamepadManager)->GetState(m_entity->GetComponent<PlayerInput>()->id);

    if ( input == nullptr )
        return ursine::Vec2(0, 0);

    if ( m_binding >= LeftStickLeft && m_binding <= LeftStickDown )
        return input->Sticks().Left();

    return input->Sticks().Right();
}

bool PlayerAction::operator==(const ActionMode& mode) 
{
    return m_actionMode == mode;
}