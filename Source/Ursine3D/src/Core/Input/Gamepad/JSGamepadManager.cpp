/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** JSGamepadManager.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "JSGamepadManager.h"
#include "UIConfig.h"

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

namespace
{
    const auto kChannelGamepadManager = "GamepadManager";

    const auto kEventGamepadButtonDown = "GamepadButtonDown";
    const auto kEventGamepadButtonUp = "GamepadButtonUp";
    const auto kEventGamepadConnected = "GamepadConnected";
    const auto kEventGamepadDisconnected = "GamepadDisconnected";
}

namespace ursine
{
    JSConstructor(JSGamepadManager)
        : m_manager( GetCoreSystem( GamepadManager ) )
        , m_browser( CefV8Context::GetEnteredContext( )->GetBrowser( ) )
    {
        m_manager->Listener( this )
            .On( GP_BUTTON_DOWN, &JSGamepadManager::onGamepadButtonDown )
            .On( GP_BUTTON_UP, &JSGamepadManager::onGamepadButtonUp )
            .On( GP_CONNECTED, &JSGamepadManager::onGamepadConnected )
            .On( GP_DISCONNECTED, &JSGamepadManager::onGamepadDisconnected );
    }

    JSGamepadManager::~JSGamepadManager(void)
    {
        m_manager->Listener( this )
            .Off( GP_BUTTON_DOWN, &JSGamepadManager::onGamepadButtonDown )
            .Off( GP_BUTTON_UP, &JSGamepadManager::onGamepadButtonUp )
            .Off( GP_CONNECTED, &JSGamepadManager::onGamepadConnected )
            .Off( GP_DISCONNECTED, &JSGamepadManager::onGamepadDisconnected );
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

    void JSGamepadManager::onGamepadButtonDown(EVENT_HANDLER(GamepadManager))
    {
        EVENT_ATTRS(GamepadManager, GamepadButtonArgs);

        Json::object btnEvent {
            { "index", static_cast<int>( args->gamepad ) },
            { "button", static_cast<int>( args->button ) },
            { "pressed", true },
            { "triggered", args->triggered }
        };

        messageBrowser( 
            kChannelGamepadManager, 
            kEventGamepadButtonDown,
            btnEvent
        );
    }

    void JSGamepadManager::onGamepadButtonUp(EVENT_HANDLER(GamepadManager))
    {
        EVENT_ATTRS(GamepadManager, GamepadButtonArgs);

        Json::object btnEvent {
            { "index", static_cast<int>( args->gamepad ) },
            { "button", static_cast<int>( args->button ) },
            { "pressed", false },
            { "triggered", args->triggered }
        };

        messageBrowser( 
            kChannelGamepadManager, 
            kEventGamepadButtonUp, 
            btnEvent 
        );
    }

    void JSGamepadManager::onGamepadConnected(EVENT_HANDLER(GamepadManager))
    {
        EVENT_ATTRS(GamepadManager, GamepadConnectionArgs);

        Json::object connectionEvent {
            { "index", static_cast<int>( args->index ) },
            { "connected", true }
        };

        messageBrowser( 
            kChannelGamepadManager, 
            kEventGamepadConnected, 
            connectionEvent 
        );
    }

    void JSGamepadManager::onGamepadDisconnected(EVENT_HANDLER(GamepadManager))
    {
        EVENT_ATTRS(GamepadManager, GamepadConnectionArgs);

        Json::object connectionEvent {
            { "index", static_cast<int>( args->index ) },
            { "connected", false }
        };

        messageBrowser( 
            kChannelGamepadManager, 
            kEventGamepadDisconnected, 
            connectionEvent 
        );
    }
}