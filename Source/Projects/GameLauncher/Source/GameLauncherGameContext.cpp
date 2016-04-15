#include "Precompiled.h"

#include "GameLauncherGameContext.h"

#include "GameLauncher.h"

#include <Application.h>
#include <GfxAPI.h>
#include <AudioSystem.h>

GameLauncherGameContext::GameLauncherGameContext(ursine::Scene *scene)
    : GameContext( scene, this )
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

float GameLauncherGameContext::GetVolume(const std::string &outputType) const
{
    auto search = m_volumeCache.find( outputType );

    return search == m_volumeCache.end( ) ? 1.0f : search->second;
}

void GameLauncherGameContext::SetVolume(float volume, const std::string &outputType)
{
    ursine::ecs::AudioSystem::SetRealTimeParameter( outputType, volume * 100.0f );

    m_volumeCache[ outputType ] = volume;
}

void GameLauncherGameContext::ExitGame(void)
{
    ursine::Application::Instance->Exit( );
}