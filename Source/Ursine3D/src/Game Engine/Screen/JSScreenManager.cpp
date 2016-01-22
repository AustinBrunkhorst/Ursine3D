/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
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

#include "JSScreenManager.h"

#include <mutex>

namespace ursine
{
    JSConstructor(JSScreenManager::JSScreenManager)
        : m_manager( gScreenManager )
    {
        
    }

    JSMethod(JSScreenManager::setScreen)
    {
        auto name = arguments[ 0 ]->GetStringValue( ).ToString( );

        std::string error;

        auto data = JsonSerializer::Serialize( arguments[ 1 ] );

        // this needs to be called on the main thread
        Timer::Create( TimeSpan::Zero ).Completed( [=]
        {
            std::lock_guard<std::mutex> lock( m_mutex );

            m_manager->SetScreen(
                gScreenManager->CreateScreen( name ), 
                data 
            );
        } );

        return CefV8Value::CreateUndefined( );
    }

    JSMethod(JSScreenManager::addOverlay)
    {
        auto name = arguments[ 0 ]->GetStringValue( ).ToString( );

        std::string error;

        auto data = JsonSerializer::Serialize( arguments[ 1 ] );

        // this needs to be called on the main thread
        Timer::Create( TimeSpan::Zero ).Completed( [=]
        {
            std::lock_guard<std::mutex> lock( m_mutex );

            m_manager->AddOverlay( name, data );
        });

        return CefV8Value::CreateUndefined( );
    }

    JSMethod(JSScreenManager::removeScreen)
    {
        auto id = arguments[ 0 ]->GetIntValue( );

        std::lock_guard<std::mutex> lock( m_mutex );

        m_manager->RemoveScreen( static_cast<ScreenID>( id ) );

        return CefV8Value::CreateUndefined( );
    }

    JSMethod(JSScreenManager::removeCurrent)
    {
        std::lock_guard<std::mutex> lock( m_mutex );

        m_manager->RemoveCurrent( );

        return CefV8Value::CreateUndefined( );
    }

    JSMethod(JSScreenManager::messageScreen)
    {
        std::lock_guard<std::mutex> lock( m_mutex );

        auto data = JsonSerializer::Serialize( arguments[ 2 ] );

        m_manager->MessageScreen(
            // id
            arguments[ 0 ]->GetIntValue( ),
            // message
            arguments[ 1 ]->GetStringValue( ),
            // data
            data 
        );

        return CefV8Value::CreateUndefined( );
    }

    JSMethod(JSScreenManager::getFocusedScreen)
    {
        return CefV8Value::CreateUInt( m_manager->GetFocusedScreen( ) );
    }
}