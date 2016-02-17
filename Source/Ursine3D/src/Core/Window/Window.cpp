/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Window.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "Window.h"

#include "WindowManager.h"

#include <SDL_syswm.h>
#include <SDL_image.h>

namespace ursine
{
     Window::Window(WindowManager *manager, InternalWindowHandle handle)
        : EventDispatcher( this )
        , m_isFocused( true )
        , m_isFullscreen( false )
        , m_isShown( false )
        , m_isMaximized( false )
        , m_id( SDL_GetWindowID( handle ) )
        , m_manager( manager )
        , m_handle( handle )
    {
        int width, height;

        SDL_GetWindowSize( handle, &width, &height );

        m_size.Set( static_cast<float>( width ), static_cast<float>( height ) );
    }

    Window::~Window(void)
    {
        SDL_DestroyWindow( m_handle );
    }

    void Window::SetBordered(bool bordered)
    {
        SDL_SetWindowBordered( m_handle, bordered ? SDL_TRUE : SDL_FALSE );
    }

    void Window::SetResizable(bool resizable)
    {
#if defined(PLATFORM_WINDOWS)

        auto hwnd = static_cast<HWND>( GetPlatformHandle( ) );

        auto styleFlags = GetWindowLong( hwnd, GWL_STYLE );

        if (resizable)
            utils::FlagSet( styleFlags, WS_SIZEBOX );
        else
            utils::FlagUnset( styleFlags, WS_SIZEBOX );

        SetWindowLong( hwnd, GWL_STYLE, styleFlags );

#endif
    }

    const Vec2 &Window::GetSize(void) const
    {
        return m_size;
    }

    void Window::SetSize(const Vec2 &size)
    {
        auto centerOld = m_location + m_size * 0.5f;

        auto width = static_cast<int>( size.X() );
        auto height = static_cast<int>( size.Y() );

        SDL_SetWindowSize( m_handle, width, height );

        m_size = size;

        SetLocation( centerOld - size * 0.5f );

        WindowResizeArgs resize( width, height );

        Dispatch( WINDOW_RESIZE, &resize );
    }

    const Vec2 &Window::GetLocation(void) const
    {
        return m_location;
    }

    void Window::SetLocation(const Vec2 &location)
    {
        SDL_SetWindowPosition(m_handle,
            static_cast<int>( location.X( ) ),
            static_cast<int>( location.Y( ) )
        );

        m_location = location;
    }

    void Window::SetLocationCentered(void)
    {
        SDL_SetWindowPosition( m_handle,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED );

        int x, y;

        SDL_GetWindowPosition( m_handle, &x, &y );

        m_location.Set( 
            static_cast<float>( x ), 
            static_cast<float>( y ) 
        );
    }

    bool Window::IsFocused(void) const
    {
        return m_isFocused;
    }

    InternalWindowHandle Window::GetInternalHandle(void) const
    {
        return m_handle;
    }

    void *Window::GetPlatformHandle(void)
    {
        SDL_SysWMinfo info;

        SDL_GetVersion( &info.version );

        SDL_GetWindowWMInfo( m_handle, &info );

#if defined(PLATFORM_WINDOWS)

        return info.info.win.window; 

#else
        return nullptr;

#endif
    }

    uint32 Window::GetID(void) const
    {
        return m_id;
    }

    bool Window::IsFullScreen(void) const
    {
        return m_isFullscreen;
    }

    void Window::SetFullScreen(bool fullscreen)
    {
        SDL_SetWindowDisplayMode( m_handle, nullptr );
        SDL_SetWindowFullscreen( m_handle, fullscreen ? SDL_WINDOW_FULLSCREEN : 0 );

        if (!fullscreen)
            SetLocationCentered( );

        m_isFullscreen = fullscreen;
    }

    bool Window::IsMaximized(void) const
    {
        return m_isMaximized;
    }

    void Window::SetMaximized(bool maximized)
    {
        if (maximized)
            SDL_MaximizeWindow( m_handle );
        else
            SDL_RestoreWindow( m_handle );

        m_isMaximized = maximized;
    }

    void Window::SetMinimized(bool minimized)
    {
        if (minimized)
            SDL_MinimizeWindow( m_handle );
        else
            SDL_RestoreWindow( m_handle );
    }

    bool Window::IsShown(void) const
    {
        return m_isShown;
    }

    void Window::Show(bool show)
    {
        if (show)
        {
            SDL_ShowWindow( m_handle );
        }
        else
        {
            SDL_HideWindow( m_handle );
        }

        m_isShown = show;
    }

    void Window::SetIcon(const std::string &filename)
    {
        auto surface = IMG_Load( filename.c_str( ) );

        SDL_SetWindowIcon( m_handle, surface );

        SDL_FreeSurface( surface );
    }

    int Window::GetDisplayIndex(void) const
    {
        return SDL_GetWindowDisplayIndex( m_handle );
    }

    std::vector<SDL_DisplayMode> Window::GetAvailableDisplayModes(void) const
    {
        std::vector<SDL_DisplayMode> modes;

        auto displayIndex = GetDisplayIndex( );

        auto modeCount = SDL_GetNumDisplayModes( displayIndex );

        for (int i = 0; i < modeCount; ++i)
        {
            SDL_DisplayMode mode { SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0 };

            SDL_GetDisplayMode( displayIndex, i, &mode );

            modes.emplace_back( mode );
        }

        return modes;
    }

    SDL_DisplayMode Window::GetDisplayMode(void) const
    {
        SDL_DisplayMode mode;

        SDL_GetWindowDisplayMode( m_handle, &mode );

        return mode;
    }

    void Window::SetDisplayMode(const SDL_DisplayMode &mode)
    {
        SetSize( {
            static_cast<float>( mode.w ),
            static_cast<float>( mode.h )
        } );

        auto fullscreen = m_isFullscreen;

        // this is absurd, but it works 0_o
        SetFullScreen( !fullscreen );

        SDL_SetWindowDisplayMode( m_handle, &mode );

        SetFullScreen( fullscreen );
    }

    void Window::Destroy(void)
    {
        m_manager->destroy( this );
    }

    void Window::onWindowEvent(const SDL_WindowEvent &e)
    {
        switch (e.event)
        {
            case SDL_WINDOWEVENT_CLOSE:
            {
                Destroy( );
            }
            break;
            case SDL_WINDOWEVENT_RESIZED:
            {
                auto width = e.data1;
                auto height = e.data2;

                m_size.Set( 
                    static_cast<float>( width ), 
                    static_cast<float>( height ) 
                );

                WindowResizeArgs resize( width, height );

                Dispatch( WINDOW_RESIZE, &resize );
            }
                break;
            case SDL_WINDOWEVENT_MOVED:
            {
                m_location.Set( 
                    static_cast<float>( e.data1 ), 
                    static_cast<float>( e.data2 )
                );
            }
                break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
            case SDL_WINDOWEVENT_FOCUS_LOST:
            {
                m_isFocused = (e.event == SDL_WINDOWEVENT_FOCUS_GAINED);

                WindowFocusArgs focus( m_isFocused );

                Dispatch( WINDOW_FOCUS_CHANGED, &focus );
            }
                break;
        }
    }
}