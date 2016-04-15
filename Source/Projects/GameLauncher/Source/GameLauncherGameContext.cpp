#include "Precompiled.h"

#include "GameLauncherGameContext.h"

#include "GameLauncher.h"

#include <Application.h>
#include <GfxAPI.h>

GameLauncherGameContext::GameLauncherGameContext(void)
    : GameContext( this )
    , m_launcher( GetCoreSystem( GameLauncher ) ) { }

bool GameLauncherGameContext::GetWindowFullScreen(void) const
{
    return m_launcher->GetWindow( )->IsFullScreen( );
}

void GameLauncherGameContext::SetWindowFullScreen(bool fullScreen)
{
    auto window = m_launcher->GetWindow( );

    if (fullScreen)
    {
        SDL_DisplayMode fullScreenMode;

        SDL_GetDesktopDisplayMode( window->GetDisplayIndex( ), &fullScreenMode );

        window->SetSize( {
            static_cast<float>( fullScreenMode.w ),
            static_cast<float>( fullScreenMode.h )
        } );
    }

    window->SetFullScreen( fullScreen );

    if (!fullScreen)
    {
        window->SetSize( kDefaultWindowDimensions );
        window->SetLocationCentered( );
    }

    GetCoreSystem( ursine::graphics::GfxAPI )->SetFullscreenState( fullScreen );
}

void GameLauncherGameContext::SetVolume(float volume, const std::string &outputType) const
{
    auto scene = m_launcher->GetScene( );

    if (!scene)
        return;

    auto world = scene->GetActiveWorld( );

    if (!world)
        return;

    ursine::ecs::VolumeChangeArgs args( volume, outputType );

    world->Dispatch( ursine::ecs::WORLD_VOLUME_CHANGE, &args );
}

void GameLauncherGameContext::ExitGame(void)
{
    ursine::Application::Instance->Exit( );
}