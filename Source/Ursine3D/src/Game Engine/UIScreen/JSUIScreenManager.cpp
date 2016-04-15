/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** JSScreenManager.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "JSUIScreenManager.h"

#include "UIScreen.h"

namespace ursine
{
    JSConstructor(JSUIScreenManager::JSUIScreenManager)
        : m_manager( JSGetGlobalScreenManager( ) ) { }

    JSUIScreenManager::JSUIScreenManager(const JSUIScreenManager &rhs)
        : m_manager( rhs.m_manager ) { }

    JSMethod(JSUIScreenManager::createScreen)
    {
        if (arguments.size( ) != 3)
            JSThrow( "Invalid arguments.", nullptr );

        auto path = arguments[ 0 ]->GetStringValue( ).ToString( );
        auto inputBlocking = arguments[ 1 ]->GetBoolValue( );
        auto priority = arguments[ 2 ]->GetIntValue( );

        auto screen = m_manager->CreateScreenRemote(
            path,
            { inputBlocking, priority }
        );

        return CefV8Value::CreateUInt( screen->GetID( ) );
    }

    JSMethod(JSUIScreenManager::removeScreen)
    {
        if (arguments.size( ) != 1)
            JSThrow( "Invalid arguments.", nullptr );

        auto id = static_cast<UIScreenID>( arguments[ 0 ]->GetUIntValue( ) );

        auto screen = m_manager->GetScreen( id );

        if (!screen)
            return CefV8Value::CreateBool( false );

        m_manager->RemoveScreen( screen );

        return CefV8Value::CreateBool( true );
    }

    JSMethod(JSUIScreenManager::screenHasFocus)
    {
        if (arguments.size( ) != 1)
            JSThrow( "Invalid arguments.", nullptr );

        auto id = static_cast<UIScreenID>( arguments[ 0 ]->GetUIntValue( ) );

        auto screen = m_manager->GetScreen( id );

        return CefV8Value::CreateBool( screen ? screen->HasInputFocus( ) : false );
    }

    JSMethod(JSUIScreenManager::messageOwner)
    {
        if (arguments.size( ) != 3)
            JSThrow( "Invalid arguments.", nullptr );

        auto id = static_cast<UIScreenID>( arguments[ 0 ]->GetUIntValue( ) );

        auto screen = m_manager->GetScreen( id );

        if (!screen)
            return CefV8Value::CreateBool( false );

        auto event = arguments[ 1 ]->GetStringValue( );
        auto data = JsonSerializer::Serialize( arguments[ 2 ] );

        Application::PostMainThread( [=] {
            m_manager->MessageScreenNative(
                screen, 
                event,
                data
            );
        } );

        return CefV8Value::CreateBool( true );
    }

    JSMethod(JSUIScreenManager::messageGlobal)
    {
        if (arguments.size( ) != 2)
            JSThrow( "Invalid arguments.", nullptr );
        
        auto event = arguments[ 0 ]->GetStringValue( );
        auto data = JsonSerializer::Serialize( arguments[ 1 ] );

        Application::PostMainThread( [=] {
            m_manager->MessageGlobalNative( event, data );
        } );

        return CefV8Value::CreateBool( true );
    }
}