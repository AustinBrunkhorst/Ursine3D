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
#include <Components/PlayerIdComponent.h>
#include <Core/CoreSystem.h>
#include <Core/Input/Gamepad/GamepadManager.h>
#include <Core/Input/Keyboard/KeyboardManager.h>
#include <Core/Input/Mouse/MouseManager.h>

#define Threshold 0.5f


PlayerAction::PlayerAction( )
    : m_actionMode( ActionCount )
    , m_binding( BindingCount )
    , m_gamepadState( nullptr )
    , m_gamepadManager( GetCoreSystem(ursine::GamepadManager) )
    , m_keyboardManager( GetCoreSystem(ursine::KeyboardManager) )
    , m_mouseManager( GetCoreSystem(ursine::MouseManager) )
{
    
}

PlayerAction::PlayerAction(PlayerID* idComp, const ActionMode action, const InputBinding binding)
    : m_actionMode( action )
    , m_binding( binding )
    , m_playerIDComp( idComp )
    , m_gamepadState( nullptr )
    , m_gamepadManager(GetCoreSystem(ursine::GamepadManager))
    , m_keyboardManager(GetCoreSystem(ursine::KeyboardManager))
    , m_mouseManager(GetCoreSystem(ursine::MouseManager))
{

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

bool PlayerAction::WasPressed()
{    
    switch ( m_actionMode )
    {
    case Xbox:
        return EvalXboxButtons(&ursine::GamepadState::IsButtonTriggeredDown);
    case Keyboard:
        return EvalKeyboardButtons(&ursine::KeyboardManager::IsTriggeredDown, &ursine::MouseManager::IsButtonTriggeredDown);
    default:
        break;
    }

    return false;
}

bool PlayerAction::IsPressed()
{
    switch ( m_actionMode )
    {
    case Xbox:
        return EvalXboxButtons(&ursine::GamepadState::IsButtonDown);
    case Keyboard:
        return EvalKeyboardButtons(&ursine::KeyboardManager::IsDown, &ursine::MouseManager::IsButtonDown);
    default:
        break;
    }

    return false;
}

bool PlayerAction::WasReleased( )
{ 
    switch ( m_actionMode )
    {
    case Xbox:
        return EvalXboxButtons(&ursine::GamepadState::IsButtonTriggeredUp);
    case Keyboard:
        return EvalKeyboardButtons(&ursine::KeyboardManager::IsTriggeredUp, &ursine::MouseManager::IsButtonTriggeredUp);
    default:
        break;
    }

    return false;
}

bool PlayerAction::StickUp() 
{
    if ( !PrepForInput( ) )
        return false;

    switch ( m_binding )
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

bool PlayerAction::StickDown() 
{
    if ( !PrepForInput( ) )
        return false;

    switch ( m_binding )
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

bool PlayerAction::StickLeft() 
{
    if ( !PrepForInput( ) )
        return false;

    switch ( m_binding )
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

bool PlayerAction::StickRight() 
{
    if ( !PrepForInput( ) )
        return false;

    switch ( m_binding )
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

ursine::Vec2 PlayerAction::GetMouseVec(void) const
{
    ursine::Vec2 mouseVec = m_mouseManager->GetPositionDelta( ) * 0.5f;
    mouseVec.Normalize( );
    mouseVec = -mouseVec;
    return mouseVec;
}

ursine::Vec2 PlayerAction::GetAxis(void)
{
    if ( !PrepForInput( ) )
        return ursine::Vec2(0, 0);

    switch ( m_binding )
    {
    case LeftStickLeft: case LeftStickDown: case LeftStickRight: case LeftStickUp:
        return m_gamepadState->Sticks( ).Left( );
    case RightStickLeft: case RightStickDown: case RightStickRight: case RightStickUp:
        return m_gamepadState->Sticks( ).Right( );
    case MouseUp: case MouseRight: case MouseLeft: case MouseDown:
        return GetMouseVec( );
    }

    ursine::Vec2 vec(0, 0);
    
    // keyboard up
    if ( m_keyboardManager->IsDown(ursine::KEY_W) )
    {
        vec.SetY( vec.Y( ) + 1.0f );
    }
    // keyboard down
    if ( m_keyboardManager->IsDown(ursine::KEY_S) )
    {
        vec.SetY( vec.Y( ) - 1.0f );
    }
    // keyboard right
    if ( m_keyboardManager->IsDown(ursine::KEY_D) )
    {
        vec.SetX( vec.X( ) + 1.0f );
    }
    // keyboard left
    if ( m_keyboardManager->IsDown(ursine::KEY_A) )
    {
        vec.SetX( vec.X( ) - 1.0f );
    }

    return vec;
}

bool PlayerAction::operator==(const ActionMode& mode) const
{
    return m_actionMode == mode;
}

bool PlayerAction::EvalXboxButtons(bool(ursine::GamepadState::*func)(ursine::GamepadButton, float) const )
{        
    if ( !PrepForInput( ) )
        return false;

    switch ( m_binding )
    {
    case Action1:
        return (m_gamepadState->*func)(ursine::GamepadButton::BTN_A, Threshold );
    case Action2:
        return ( m_gamepadState->*func )(ursine::GamepadButton::BTN_B, Threshold );
    case Action3:
        return ( m_gamepadState->*func )(ursine::GamepadButton::BTN_X, Threshold );
    case RightTrigger:
        return ( m_gamepadState->*func )(ursine::GamepadButton::BTN_TRIGGER_RIGHT, Threshold );
    default:
        return ( m_gamepadState->*func )(ursine::GamepadButton::BTN_Y, Threshold );
    }
}

bool PlayerAction::EvalKeyboardButtons(bool( ursine::KeyboardManager::*func )(ursine::KeyboardKey), bool( ursine::MouseManager::*mfunc )( ursine::MouseButton ) const ) const
{
    switch ( m_binding )
    {
    case Action1:
          return ( m_keyboardManager->*func )( ursine::KeyboardKey::KEY_SPACE );
    case Action2:
        return ( m_keyboardManager->*func )( ursine::KeyboardKey::KEY_Q );
    case Action3:
        return ( m_keyboardManager->*func )( ursine::KeyboardKey::KEY_X );
    case RightTrigger: case LeftTrigger:
        return EvalMouseButtons(mfunc);
    default:
        return ( m_keyboardManager->*func )( ursine::KeyboardKey::KEY_R );
    }
}

bool PlayerAction::EvalMouseButtons(bool( ursine::MouseManager::*func )( ursine::MouseButton ) const) const
{
    switch( m_binding )
    {
    case RightTrigger:
        return ( m_mouseManager->*func )( ursine::MouseButton::MBTN_LEFT );
    case LeftTrigger:
        return ( m_mouseManager->*func )( ursine::MouseButton::MBTN_RIGHT );
    default:
        return false;
    }
}
