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

bool GameLauncherGameContext::IsMusicMuted(void) const
{
    return false;
}

void GameLauncherGameContext::MuteMusic(bool muted)
{
    
}

bool GameLauncherGameContext::IsAudioMuted(void) const
{
    return false;
}

void GameLauncherGameContext::MuteAudio(bool muted)
{
    
}

void GameLauncherGameContext::ExitGame(void)
{
    ursine::Application::Instance->Exit( );
}