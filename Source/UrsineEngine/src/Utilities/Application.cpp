/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Application.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "Application.h"

#include "WindowManager.h"

namespace ursine
{
    Application::Application(int argc, char *argv[])
        : m_argc( argc )
        , m_argv( argv )
        , m_isRunning( true )
    {
        // default FPS
        SetTargetFPS( 60 );

        SDL_Init( SDL_INIT_TIMER | SDL_INIT_EVENTS | 
                 SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | 
                 SDL_INIT_GAMECONTROLLER );
    }

    Application::~Application(void)
    {
        SDL_Quit( );
    }

    void Application::Run(void)
    {
        SDL_Event e;

        InternalApplicationArgs internal;

        while (m_isRunning)
        {
            m_frameRateController.FrameBegin( );

            m_dt = m_frameRateController.GetDeltaTime( );

            while (SDL_PollEvent( &e ))
            {
                if (e.type == SDL_QUIT)
                    Exit( );

                internal.data = &e;

                m_internalEvents.Dispatch( e.type, this, &internal );
            }

#ifdef CONFIG_RELEASE
            if (gWindowManager->IsFocused( ))
            {
#endif
                glClear( GL_COLOR_BUFFER_BIT );

                // @@@TODO: Dispatch update

                SDL_GL_SwapWindow( gWindowManager->GetHandle( ) );
#ifdef CONFIG_RELEASE
            }
            else
            {
                // avoid empty cycles when the application is not focused
                SDL_Delay( 120 );
            }
#endif
            m_frameRateController.FrameEnd( );
        }
    }

    void Application::Exit(void)
    {
        m_isRunning = false;
    }

    int Application::GetArgC(void) const
    {
        return m_argc;
    }

    char **Application::GetArgV(void) const
    {
        return m_argv;
    }

    EventDispatcher<uint32> &Application::GetInternalEvents(void)
    {
        return m_internalEvents;
    }

    float Application::GetDelta(void)
    {
        return m_dt;
    }

    void Application::SetTargetFPS(uint32 fps)
    {
        m_frameRateController.SetTargetFPS( fps );
    }

    uint32 Application::GetFPS(void)
    {
        return m_frameRateController.GetFPS( );
    }
}
