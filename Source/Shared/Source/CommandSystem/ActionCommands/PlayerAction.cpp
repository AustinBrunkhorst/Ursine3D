/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayerAction.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "PlayerAction.h"
#include <PlayerIdComponent.h>
#include <Core/CoreSystem.h>
#include <Core/Input/Gamepad/GamepadManager.h>
#include <Core/Input/Keyboard/KeyboardManager.h>
#include <Core/Input/Mouse/MouseManager.h>

#define Threshold 0.5f

ursine::GamepadManager*  PlayerAction::m_gamepadManager = nullptr;
ursine::KeyboardManager* PlayerAction::m_keyboardManager = nullptr;
ursine::MouseManager*    PlayerAction::m_mouseManager = nullptr;


PlayerAction::PlayerAction(void)
    : m_actionMode( ActionCount ),
    m_playerIDComp( nullptr ),
    m_gamepadState(nullptr)
{
    m_gamepadManager = GetCoreSystem( ursine::GamepadManager );
    m_keyboardManager = GetCoreSystem( ursine::KeyboardManager );
    m_mouseManager = GetCoreSystem( ursine::MouseManager );
}

PlayerAction::PlayerAction(PlayerID* idComp, ActionMode action) :
    m_actionMode( action ),
    m_playerIDComp( idComp ),
    m_gamepadState( nullptr )
{
    m_gamepadManager = GetCoreSystem( ursine::GamepadManager );
    m_keyboardManager = GetCoreSystem( ursine::KeyboardManager );
    m_mouseManager = GetCoreSystem( ursine::MouseManager );
}


void PlayerAction::SetActionMode(ActionMode mode)
{
    m_actionMode = mode;
}

void PlayerAction::SetPlayerIDComp(PlayerID* idComp)
{
    m_playerIDComp = idComp;
}


bool PlayerAction::PrepForInput(void)
{
    if ( m_actionMode == Keyboard )
        return true;

    if ( m_playerIDComp == nullptr )
        return false;

    m_gamepadState = m_gamepadManager->GetState(m_playerIDComp->GetID( ));

    if ( m_gamepadState == nullptr )
        return false;

    return true;
}

bool PlayerAction::WasPressed(const InputBinding binding)
{    
    switch ( m_actionMode )
    {
    case Xbox:
        return EvalXboxButtons(binding, &ursine::GamepadState::IsButtonTriggeredDown);
    case Keyboard:
        return EvalKeyboardButtons(binding, &ursine::KeyboardManager::IsTriggeredDown, &ursine::MouseManager::IsButtonTriggeredDown);
    default:
        break;
    }

    return false;
}

bool PlayerAction::IsPressed(const InputBinding binding)
{
    switch ( m_actionMode )
    {
    case Xbox:
        return EvalXboxButtons(binding, &ursine::GamepadState::IsButtonDown);
    case Keyboard:
        return EvalKeyboardButtons(binding, &ursine::KeyboardManager::IsDown, &ursine::MouseManager::IsButtonDown);
    default:
        break;
    }

    return false;
}

bool PlayerAction::WasReleased(const InputBinding binding)
{ 
    switch ( m_actionMode )
    {
    case Xbox:
        return EvalXboxButtons(binding, &ursine::GamepadState::IsButtonTriggeredUp);
    case Keyboard:
        return EvalKeyboardButtons(binding, &ursine::KeyboardManager::IsTriggeredUp, &ursine::MouseManager::IsButtonTriggeredUp);
    default:
        break;
    }

    return false;
}

bool PlayerAction::IsUp(const InputBinding binding)
{
    switch ( m_actionMode )
    {
    case Xbox:
        return EvalXboxButtons(binding, &ursine::GamepadState::IsButtonUp);
    case Keyboard:
        return EvalKeyboardButtons(binding, &ursine::KeyboardManager::IsTriggeredUp, &ursine::MouseManager::IsButtonUp);
    default:
        break;
    }

    return false;
}

bool PlayerAction::StickUp(const InputBinding binding)
{
    if ( !PrepForInput( ) )
        return false;

    switch ( binding )
    {
    case KeyUp:
        return m_keyboardManager->IsDown(ursine::KEY_W);
    case MouseUp:
        return m_mouseManager->GetPositionDelta( ).Y( ) > 0;
    case LeftStickUp:
        return m_gamepadState->Sticks().Left().Y() > 0;
    default:
        return m_gamepadState->Sticks().Right().Y() > 0;
    }
}

bool PlayerAction::StickDown(const InputBinding binding)
{
    if ( !PrepForInput( ) )
        return false;

    switch ( binding )
    {
    case KeyDown:
        return m_keyboardManager->IsDown(ursine::KEY_S);
    case MouseDown:
        return m_mouseManager->GetPositionDelta( ).Y( ) < 0;
    case LeftStickDown:
        return m_gamepadState->Sticks().Left().Y() < 0;
    default:
        return m_gamepadState->Sticks().Right().Y() < 0;
    }
}

bool PlayerAction::StickLeft(const InputBinding binding)
{
    if ( !PrepForInput( ) )
        return false;

    switch ( binding )
    {
    case KeyLeft:
        return m_keyboardManager->IsDown(ursine::KEY_A);
    case MouseLeft:
        return m_mouseManager->GetPositionDelta( ).X( ) < 0;
    case LeftStickLeft:
        return m_gamepadState->Sticks().Left().X() < 0;
    default:
        return m_gamepadState->Sticks().Right().X() < 0;
    }
}

bool PlayerAction::StickRight(const InputBinding binding)
{
    if ( !PrepForInput( ) )
        return false;

    switch ( binding )
    {
    case KeyRight:
        return m_keyboardManager->IsDown(ursine::KEY_D);
    case MouseRight:
        return m_mouseManager->GetPositionDelta( ).X( ) > 0;
    case LeftStickRight:
        return m_gamepadState->Sticks().Left().X() > 0;
    default:
        return m_gamepadState->Sticks().Right().X() > 0;
    }
}

void PlayerAction::GetMouseVec(ursine::Vec2& vecToSet) const
{
    vecToSet = m_mouseManager->GetPositionDelta( );
    vecToSet.Normalize( );
    vecToSet.X( ) = -vecToSet.X( );
}

void PlayerAction::GetAxis(const InputBinding binding, ursine::Vec2& vecToSet)
{
    vecToSet.Zero( );

    if ( !PrepForInput( ) )
        return;

    switch ( binding )
    {
    case LeftStickLeft: case LeftStickDown: case LeftStickRight: case LeftStickUp:
        vecToSet = m_gamepadState->Sticks( ).Left( );
        break;
    case RightStickLeft: case RightStickDown: case RightStickRight: case RightStickUp:
        vecToSet = m_gamepadState->Sticks( ).Right( );
        break;
    case MouseUp: case MouseRight: case MouseLeft: case MouseDown:
        GetMouseVec(vecToSet);
        break;
    default:
        // keyboard up
        if ( m_keyboardManager->IsDown(ursine::KEY_W) )
        {
            vecToSet.SetY(vecToSet.Y( ) + 1.0f);
        }
        // keyboard down
        if ( m_keyboardManager->IsDown(ursine::KEY_S) )
        {
            vecToSet.SetY(vecToSet.Y( ) - 1.0f);
        }
        // keyboard right
        if ( m_keyboardManager->IsDown(ursine::KEY_D) )
        {
            vecToSet.SetX(vecToSet.X( ) + 1.0f);
        }
        // keyboard left
        if ( m_keyboardManager->IsDown(ursine::KEY_A) )
        {
            vecToSet.SetX(vecToSet.X( ) - 1.0f);
        }
    }
}

bool PlayerAction::operator==(const ActionMode& mode) const
{
    return m_actionMode == mode;
}

bool PlayerAction::EvalXboxButtons(const InputBinding binding, bool(ursine::GamepadState::*func)(ursine::GamepadButton, float) const )
{        
    if ( !PrepForInput( ) )
        return false;

    switch ( binding )
    {
    case Action1:
        return (m_gamepadState->*func)(ursine::GamepadButton::BTN_A, Threshold );
    case Action2:
        return ( m_gamepadState->*func )(ursine::GamepadButton::BTN_B, Threshold );
    case Action3:
        return ( m_gamepadState->*func )(ursine::GamepadButton::BTN_X, Threshold );
    case RightTrigger:
        return ( m_gamepadState->*func )(ursine::GamepadButton::BTN_TRIGGER_RIGHT, Threshold );
    case LeftTrigger:
        return ( m_gamepadState->*func )( ursine::GamepadButton::BTN_TRIGGER_LEFT, Threshold );
    default:
        return ( m_gamepadState->*func )(ursine::GamepadButton::BTN_Y, Threshold );
    }
}

bool PlayerAction::EvalKeyboardButtons(const InputBinding binding, bool( ursine::KeyboardManager::*func )(ursine::KeyboardKey), bool( ursine::MouseManager::*mfunc )( ursine::MouseButton ) const ) const
{
    switch ( binding )
    {
    case Action1:
          return ( m_keyboardManager->*func )( ursine::KeyboardKey::KEY_SPACE );
    case Action2:
        return ( m_keyboardManager->*func )( ursine::KeyboardKey::KEY_Q );
    case Action3:
        return ( m_keyboardManager->*func )( ursine::KeyboardKey::KEY_X );
    case RightTrigger: case LeftTrigger:
        return EvalMouseButtons(binding, mfunc);
    default:
        return ( m_keyboardManager->*func )( ursine::KeyboardKey::KEY_R );
    }
}

bool PlayerAction::EvalMouseButtons(const InputBinding binding, bool( ursine::MouseManager::*func )( ursine::MouseButton ) const) const
{
    switch( binding )
    {
    case RightTrigger:
        return ( m_mouseManager->*func )( ursine::MouseButton::MBTN_LEFT );
    case LeftTrigger:
        return ( m_mouseManager->*func )( ursine::MouseButton::MBTN_RIGHT );
    default:
        return false;
    }
}

