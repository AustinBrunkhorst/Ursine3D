/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** JSKeyboardManager.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "JSKeyboardManager.h"

#define KeyStateMethodSetup                                          \
    auto key =                                                       \
        static_cast<KeyboardKey>( arguments[ 0 ]->GetUIntValue( ) ); \

namespace
{
    const auto kChannelKeyboardManager = "KeyboardManager";

    const auto kEventKeyboardKeyDown = "KeyboardKeyDown";
    const auto kEventKeyboardKeyUp = "KeyboardKeyUp";
}

namespace ursine
{
    JSConstructor(JSKeyboardManager)
        : m_manager( GetCoreSystem( KeyboardManager ) )
    {
        m_manager->Listener( this )
            .On( KM_KEY_DOWN, &JSKeyboardManager::onKeyboardKeyDown )
            .On( KM_KEY_UP, &JSKeyboardManager::onKeyboardKeyUp );

        static const auto thisType = decltypeof( *this );

        auto value = context->GetValue( thisType.GetName( ) );

        // create static instance value
        value->SetValue( "instance", thisContext, V8_PROPERTY_ATTRIBUTE_DONTDELETE );
    }

    JSKeyboardManager::~JSKeyboardManager(void)
    {
        m_manager->Listener( this )
            .Off( KM_KEY_DOWN, &JSKeyboardManager::onKeyboardKeyDown )
            .Off( KM_KEY_UP, &JSKeyboardManager::onKeyboardKeyUp );
    }

    JSMethod(JSKeyboardManager::isKeyDown) const
    {
        KeyStateMethodSetup;

        return CefV8Value::CreateBool( m_manager->IsDown( key ) );
    }

    JSMethod(JSKeyboardManager::isKeyUp) const
    {
        KeyStateMethodSetup;

        return CefV8Value::CreateBool( m_manager->IsUp( key ) );
    }

    JSMethod(JSKeyboardManager::isKeyTriggeredDown) const
    {
        KeyStateMethodSetup;

        return CefV8Value::CreateBool( m_manager->IsTriggeredDown( key ) );
    }

    JSMethod(JSKeyboardManager::isKeyTriggeredUp) const
    {
        KeyStateMethodSetup;

        return CefV8Value::CreateBool( m_manager->IsTriggeredUp( key ) );
    }

    void JSKeyboardManager::onKeyboardKeyDown(EVENT_HANDLER(KeyboardManager))
    {
        EVENT_ATTRS(KeyboardManager, KeyboardKeyArgs);

        Json::object keyEvent {
            { "key", args->key },
            { "pressed", args->state },
            { "triggered", args->repeat == 0 }
        };

        messageBrowser(
            kChannelKeyboardManager,
            kEventKeyboardKeyDown,
            keyEvent 
        );
    }

    void JSKeyboardManager::onKeyboardKeyUp(EVENT_HANDLER(KeyboardManager))
    {
        EVENT_ATTRS(KeyboardManager, KeyboardKeyArgs);

        Json::object keyEvent {
            { "key", args->key },
            { "pressed", args->state },
            { "triggered", false }
        };

        messageBrowser(
            kChannelKeyboardManager,
            kEventKeyboardKeyUp,
            keyEvent 
        );
    }
}
