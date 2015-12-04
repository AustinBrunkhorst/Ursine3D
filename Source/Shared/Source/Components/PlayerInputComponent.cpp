#include "Precompiled.h"

#include "PlayerInputComponent.h"

#include <MouseManager.h>
#include <KeyboardManager.h>
#include <GamepadManager.h>

NATIVE_COMPONENT_DEFINITION( PlayerInput );

using namespace ursine;

PlayerInput::PlayerInput( void )
    : BaseComponent( )
    , id( 0 )
    , keyboard( false )
    , triggerDepressValue( 0.1f )
    , triggerPressValue( 0.9f )
{ }

bool PlayerInput::Jump(void)
{
    if (keyboard)
    {
        return GetCoreSystem( KeyboardManager )->IsTriggeredDown( KEY_SPACE );
    }
    else
    {
        auto *state = GetCoreSystem( GamepadManager )->GetState( id );

        if (state)
            return state->IsButtonTriggeredDown( BTN_A );
        else
            return false;
    }
}

int PlayerInput::KillTeamCheat(void)
{
    auto *kbManager = GetCoreSystem(KeyboardManager);

    if (kbManager->IsTriggeredDown(KEY_F9))
    {
        return 0;
    }
    if(kbManager->IsTriggeredDown(KEY_F10))
    {
        return 1;
    }
    
    return -1;
}

ursine::Vec2 PlayerInput::MoveDir(void)
{
    if (keyboard)
    {
        auto *keyboard = GetCoreSystem( KeyboardManager );
        Vec2 dir;

        if (keyboard->IsDown( KEY_A ))
            dir.X( ) -= 1.0f;
        if (keyboard->IsDown( KEY_D ))
            dir.X( ) += 1.0f;
        if (keyboard->IsDown( KEY_W ))
            dir.Y( ) += 1.0f;
        if (keyboard->IsDown( KEY_S ))
            dir.Y( ) -= 1.0f;

        return dir;
    }
    else
    {
        auto *state = GetCoreSystem( GamepadManager )->GetState( id );

        if (state)
            return state->Sticks( ).Left( );
        else
            return Vec2::Zero( );
    }
}

ursine::Vec2 PlayerInput::LookDir(void)
{
    if (keyboard)
    {
        return GetCoreSystem( MouseManager )->GetPositionDelta( );
    }
    else
    {
        auto *state = GetCoreSystem( GamepadManager )->GetState( id );

        if (state)
            return state->Sticks( ).Right( );
        else
            return Vec2::Zero( );
    }
}

bool PlayerInput::Fire()
{
    if ( keyboard )
    {
        auto *mouse = GetCoreSystem( MouseManager );

        return mouse->IsButtonTriggeredDown( MouseButton::MBTN_LEFT );
    }

    auto *state = GetCoreSystem( GamepadManager )->GetState( id );

    if ( state )
    {
        return state->Triggers( ).Right( ) > triggerPressValue;
    }

    return false;
}

bool PlayerInput::ResetTrigger()
{
    if ( keyboard )
    {
        auto *mouse = GetCoreSystem( MouseManager );

        return !mouse->IsButtonTriggeredDown( MouseButton::MBTN_LEFT );
    }

    auto *state = GetCoreSystem( GamepadManager )->GetState( id );

    if ( state )
    {
        return state->Triggers( ).Right( ) < triggerDepressValue;
    }

    return false;
}