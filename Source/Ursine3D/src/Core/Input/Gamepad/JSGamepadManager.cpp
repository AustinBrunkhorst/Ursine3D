#include "UrsinePrecompiled.h"

#include "JSGamepadManager.h"

#define ButtonStateMethodSetup                                               \
    auto index =                                                             \
        static_cast<GamepadIndex>( arguments[ 0 ]->GetUIntValue( ) );        \
    auto *state = m_manager->GetState( index );                              \
    if (!state)                                                              \
        return CefV8Value::CreateBool( false );                              \
    auto button =                                                            \
        static_cast<GamepadButton>( arguments[ 1 ]->GetUIntValue( ) );       \
    float threshold;                                                         \
    if (arguments.size( ) == 3)                                              \
        threshold = static_cast<float>( arguments[ 2 ]->GetDoubleValue( ) ); \
    else                                                                     \
        threshold = 0.5f;                                                    \

namespace ursine
{
    JSConstructor(JSGamepadManager)
        : m_manager( GetCoreSystem( GamepadManager ) )
    {
        
    }

    JSMethod(JSGamepadManager::numConnected) const
    {
        return CefV8Value::CreateInt( m_manager->NumConnected( ) );
    }

    JSMethod(JSGamepadManager::isConnected) const
    {
        auto index = 
            static_cast<GamepadIndex>( arguments[ 0 ]->GetUIntValue( ) );

        return CefV8Value::CreateBool( 
            m_manager->GetState( index ) != nullptr 
        );
    }

    JSMethod(JSGamepadManager::isButtonDown) const
    {
        ButtonStateMethodSetup;

        return CefV8Value::CreateBool( state->IsButtonDown( button ) );
    }

    JSMethod(JSGamepadManager::isButtonUp) const
    {
        ButtonStateMethodSetup;

        return CefV8Value::CreateBool( state->IsButtonUp( button ) );
    }

    JSMethod(JSGamepadManager::isButtonTriggeredDown) const
    {
        ButtonStateMethodSetup;

        return CefV8Value::CreateBool( state->IsButtonTriggeredDown( button ) );
    }

    JSMethod(JSGamepadManager::isButtonTriggeredUp) const
    {
        ButtonStateMethodSetup;

        return CefV8Value::CreateBool( state->IsButtonTriggeredUp( button ) );
    }
}