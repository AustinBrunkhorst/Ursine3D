/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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

#include "CoreSystem.h"

#include "GfxAPI.h"

namespace ursine
{
    namespace
    {
        graphics::GfxAPI *gGraphics = nullptr;
    }

    Application *Application::Instance = nullptr;

    Application::Application(int argc, char *argv[])
        : m_isRunning( true )
        , m_isActive( true )
        , m_argc( argc )
        , m_argv( argv )
    {
        Instance = this;

        // set the executable directory as the working directory
        fs::current_path( 
            fs::path( argv[ 0 ] ).parent_path( ) 
        );

        // default FPS
        m_frameRateController.SetTargetFPS( 144 );

        // default sub systems
        SDL_InitSubSystem( SDL_INIT_TIMER | SDL_INIT_EVENTS );

        auto systemType = typeof( core::CoreSystem );

        UAssert( systemType.IsValid( ), 
            "Unable to load core::CoreSystem type.\n"
            "Was this included in the reflection parser?" );

        for (auto &derived : systemType.GetDerivedClasses( ))
        {
            auto id = derived.GetStaticField( "SystemID" );

            UAssert( id.IsValid( ),
                "Core system \"%s\" doesn't have static SystemID field.",
                derived.GetName( ).c_str( ) );

            auto index = m_systems.size( );

            id.SetValue( index );

            auto &ctor = derived.GetDynamicConstructor( );

            UAssert( ctor.IsValid( ),
              "Core system \"%s\" doesn't have a default constructor.",
              derived.GetName( ).c_str( ) );

            auto system = ctor.Invoke( );

            UAssert( system.IsValid( ), 
                "Unable to constructor core system \"%s\".",
                derived.GetName( ).c_str( ) );

            m_systems.emplace_back(
                system.GetValue<core::CoreSystem*>( ) 
            );
        }

        // initialize systems after all have been constructed
        for (auto &system : m_systems)
            system->OnInitialize( );

        gGraphics = CoreSystem<graphics::GfxAPI>( );
    }

    Application::~Application(void)
    {
        // make sure any systems cross referencing 
        // other systems have valid pointers
        for (auto *system : m_systems)
            system->OnRemove( );

        for (auto *system : m_systems)
            delete system;

        SDL_Quit( );

        Instance = nullptr;
    }

    void Application::Run(void)
    {
        SDL_Event e;

        PlatformEventArgs internal;

        internal.data = &e;

        while (m_isRunning)
        {
            m_frameRateController.FrameBegin( );

            m_dt = m_frameRateController.GetDeltaTime( );

            while (SDL_PollEvent( &e ))
            {
                if (e.type == SDL_QUIT)
                    Exit( );

                m_platformEvents.Dispatch( e.type, this, &internal );
            }

            if (m_isActive)
            {
                gGraphics->StartFrame( );

				executeMainThreadCallbacks( );

                Dispatch( APP_UPDATE, this, EventArgs::Empty );

                gGraphics->EndFrame( );
            }
            else
            {
                // avoid empty cycles when the application is not active
                SDL_Delay( 120 );
            }

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

    bool Application::IsActive(void) const
    {
        return m_isActive;
    }

    void Application::SetActive(bool active)
    {
        m_isActive = active;
    }

    DeltaTime Application::GetDeltaTime(void) const
    {
        return m_dt;
    }

    FrameRateController &Application::GetFrameRateController(void)
    {
        return m_frameRateController;
    }

    EventDispatcher<uint32> &Application::GetPlatformEvents(void)
    {
        return m_platformEvents;
    }

	void Application::PostMainThread(MainThreadCallback callback)
	{
		std::lock_guard<std::mutex> lock( Instance->m_mutex );

		Instance->m_mainThreadCallbacks.push_back( callback );
	}

	void Application::executeMainThreadCallbacks(void)
    {
		decltype( m_mainThreadCallbacks ) copy;

		// lock the vector
		{
			std::lock_guard<std::mutex> lock( m_mutex );

            copy = std::move( m_mainThreadCallbacks );
		}

		// iterate through all callbacks and execute them
		for (auto &callback : copy)
		{
			callback( );
		}
    }
}
