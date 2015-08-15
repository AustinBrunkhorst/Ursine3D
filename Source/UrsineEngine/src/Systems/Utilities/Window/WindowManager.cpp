#include "UrsinePrecompiled.h"

#include "WindowManager.h"
#include "GamepadManager.h"

#include <SDL_video.h>
#include <SDL_syswm.h>

namespace Ursine
{
    WindowManager *gWindowManager = nullptr;

    WindowManager::WindowManager(const char *title, int width, int height,
        uint32 window_flags, int x, int y)
        : EventDispatcher( this )
        , m_isFocused( true )
        , m_isFullscreen( false )
        , m_isShown( false )
    {
        gWindowManager = this;

        UAssert(gApplication != nullptr,
            "Application instance doesn't exist.");

        // create a window
        m_handle = SDL_CreateWindow( title, x, y, width, height,
            SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN | window_flags );

        if (Utils::IsFlagSet( window_flags, SDL_WINDOW_FULLSCREEN ))
            m_isFullscreen = true;

        if (Utils::IsFlagSet( window_flags, SDL_WINDOW_FULLSCREEN_DESKTOP ))
            m_isFullscreen = true;

        // create a GL context with the window
        m_glContext = SDL_GL_CreateContext( m_handle );

        gApplication->GetInternalEvents( ).Listener( this )
            .On( SDL_WINDOWEVENT, &WindowManager::onWindowEvent );

        // enable experimental features
        glewExperimental = GL_TRUE;

        // initialize GLEW
        glewInit( );

        UAssert(glewIsSupported( "GL_VERSION_3_3" ),
            "Opengl 3.3+ is required.");

        int a, b;

        SDL_GetWindowSize( m_handle, &a, &b );

        m_size.Set( a, b );

        SDL_GetWindowPosition( m_handle, &a, &b );

        m_location.Set( a, b );
    }

    WindowManager::~WindowManager(void)
    {
        gApplication->GetInternalEvents( ).Listener( this )
            .Off( SDL_WINDOWEVENT, &WindowManager::onWindowEvent );

        SDL_GL_DeleteContext( m_glContext );

        SDL_DestroyWindow( m_handle );

        gWindowManager = nullptr;
    }

    const Vector2 &WindowManager::GetSize(void) const
    {
        return m_size;
    }

    void WindowManager::SetSize(const Vector2 &size)
    {
        auto centerOld = m_location + m_size * 0.5f;

        auto width = static_cast<int>( size.x );
        auto height = static_cast<int>( size.y );

        SDL_SetWindowSize( m_handle, width, height );

        m_size = size;

        SetLocation( centerOld - size * 0.5f );

        WindowResizeArgs resize( width, height );

        Dispatch( WM_RESIZE, &resize );
    }

    const Vector2 &WindowManager::GetLocation(void) const
    {
        return m_location;
    }

    void WindowManager::SetLocation(const Vector2 &location)
    {
        SDL_SetWindowPosition(m_handle,
            static_cast<int>( location.x ),
            static_cast<int>( location.y )
        );

        m_location = location;
    }

    void WindowManager::SetLocationCentered(void)
    {
        SDL_SetWindowPosition( m_handle,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED );

        int x, y;

        SDL_GetWindowPosition( m_handle, &x, &y );

        m_location.Set( x, y );
    }

    bool WindowManager::IsFocused(void) const
    {
        return m_isFocused;
    }

    WindowHandle *WindowManager::GetHandle(void)
    {
        return m_handle;
    }

    void *WindowManager::GetPlatformHandle(void)
    {
        SDL_SysWMinfo info;

        SDL_GetVersion( &info.version );

        SDL_GetWindowWMInfo( m_handle, &info );

        URSINE_TODO("x-platform")
        return info.info.win.window;
    }

    bool WindowManager::IsFullScreen(void) const
    {
        return m_isFullscreen;
    }

    void WindowManager::SetFullScreen(bool fullscreen)
    {
        SDL_SetWindowDisplayMode( m_handle, nullptr );
        SDL_SetWindowFullscreen( m_handle, fullscreen ? SDL_WINDOW_FULLSCREEN : 0 );

        if (!fullscreen)
            SetLocationCentered( );

        m_isFullscreen = fullscreen;
    }

    bool WindowManager::IsShown(void) const
    {
        return m_isShown;
    }

    void WindowManager::Show(bool show)
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

    const Color &WindowManager::GetBackgroundColor(void) const
    {
        return m_bgColor;
    }

    URSINE_TODO("This should probably be managed in the graphics system.")
    void WindowManager::SetBackgroundColor(const Color &color)
    {
        m_bgColor = color;

        glClearColor( color.r, color.g, color.b, color.a );
    }

    int WindowManager::GetDisplayIndex(void) const
    {
        return SDL_GetWindowDisplayIndex( m_handle );
    }

    std::vector<SDL_DisplayMode> WindowManager::GetAvailableDisplayModes(void) const
    {
        std::vector<SDL_DisplayMode> modes;

        auto displayIndex = GetDisplayIndex( );

        auto modeCount = SDL_GetNumDisplayModes( displayIndex );

        for (int i = 0; i < modeCount; ++i)
        {
            SDL_DisplayMode mode = { SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0 };

            SDL_GetDisplayMode( displayIndex, i, &mode );

            modes.emplace_back( mode );
        }

        return modes;
    }

    SDL_DisplayMode WindowManager::GetDisplayMode(void) const
    {
        SDL_DisplayMode mode;

        SDL_GetWindowDisplayMode( m_handle, &mode );

        return mode;
    }

    void WindowManager::SetDisplayMode(const SDL_DisplayMode &mode)
    {
        SetSize({
            static_cast<float>( mode.w ),
            static_cast<float>( mode.h )
        });

        auto fullscreen = m_isFullscreen;

        // this is absurd, but it works 0_o
        SetFullScreen( !fullscreen );

        SDL_SetWindowDisplayMode( m_handle, &mode );

        SetFullScreen( fullscreen );
    }

    void WindowManager::onWindowEvent(EVENT_HANDLER(Application))
    {
        EVENT_ATTRS(Application, InternalApplicationArgs);

        switch (args->data->window.event)
        {
            case SDL_WINDOWEVENT_RESIZED:
            {
                int width = args->data->window.data1;
                int height = args->data->window.data2;

                m_size.Set( width, height );

                WindowResizeArgs e( width, height );

                Dispatch( WM_RESIZE, &e );
            }
            case SDL_WINDOWEVENT_MOVED:
            {
                m_location.Set( args->data->window.data1, args->data->window.data2 );
            }
                break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
            case SDL_WINDOWEVENT_FOCUS_LOST:
            {
                m_isFocused = args->data->window.event
                    == SDL_WINDOWEVENT_FOCUS_GAINED;

                if (!m_isFocused)
                    gGamepadManager->StopAllVibration( );

                WindowFocusArgs e( m_isFocused );

                Dispatch( WM_FOCUS_CHANGED, &e );
            }
                break;
        }
    }

    template<>
    Json JsonSerializer::Serialize(SDL_DisplayMode &instance)
    {
        return Json::object {
            { "width", instance.w },
            { "height", instance.h },
            { "refreshRate", instance.refresh_rate }
        };
    }

    template<>
    void JsonSerializer::Deserialize(const Json &data, SDL_DisplayMode &out)
    {
        out.driverdata = nullptr;
        out.format = 0;

        out.w = data[ "width" ].int_value( );
        out.h = data[ "height" ].int_value( );

        out.refresh_rate = data[ "refreshRate" ].int_value( );
    }
}
