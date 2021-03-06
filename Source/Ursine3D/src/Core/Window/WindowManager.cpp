/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** WindowManager.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "WindowManager.h"

#include <SDL_video.h>

namespace ursine
{
    CORE_SYSTEM_DEFINITION( WindowManager );

    WindowManager::WindowManager(void)
        : EventDispatcher( this )
    {
        Application::Instance->GetPlatformEvents( )
            .Connect( SDL_WINDOWEVENT, this, &WindowManager::onWindowEvent );
    }

    WindowManager::~WindowManager(void)
    {
        Application::Instance->GetPlatformEvents( )
            .Disconnect( SDL_WINDOWEVENT, this, &WindowManager::onWindowEvent );

        for (auto it = m_created.begin( ); it != m_created.end( );)
        {
            auto window = it->second;

            SDL_DestroyWindow( window->m_handle );

            it = m_created.erase( it );
        }
    }

    Window::Handle WindowManager::AddWindow(const std::string &title, const Vec2 &location, const Vec2 &size, uint32 flags)
    {
        auto *handle = SDL_CreateWindow(
            title.c_str( ), 
            static_cast<int>( location.X( ) ), 
            static_cast<int>( location.Y( ) ), 
            static_cast<int>( size.X( ) ), 
            static_cast<int>( size.Y( ) ),
            SDL_WINDOW_HIDDEN | SDL_WINDOW_ALLOW_HIGHDPI | flags
        );

        auto window = Window::Handle( new Window( this, handle ) );

        if (utils::IsFlagSet( flags, SDL_WINDOW_FULLSCREEN ))
            window->m_isFullscreen = true;

        if (utils::IsFlagSet( flags, SDL_WINDOW_FULLSCREEN_DESKTOP ))
            window->m_isFullscreen = true;

        m_created[ window->m_id ] = window;

        return window;
    }

    Window::Handle WindowManager::CreateFrom(const void *data)
    {
        auto *handle = SDL_CreateWindowFrom( data );

        // failed to create handle
        if (!handle)
            return nullptr;

        auto window = Window::Handle( new Window( this, handle ) );

        m_created[ window->m_id ] = window;

        return window;
    }

    Window::Handle WindowManager::GetWindow(uint32 windowID)
    {
        auto search = m_created.find( windowID );

        if (search == m_created.end( ))
            return nullptr;

        return search->second;
    }

    void WindowManager::destroy(Window *window)
    {
        m_created.erase( window->m_id );

        SDL_DestroyWindow( window->m_handle );

        window->m_handle = nullptr;
    }

    void WindowManager::onWindowEvent(EVENT_HANDLER(Application))
    {
        EVENT_ATTRS(Application, PlatformEventArgs);

        auto window = m_created.find( args->data->window.windowID );

        if (window != m_created.end( ))
            window->second->onWindowEvent( args->data->window );
    }
}
