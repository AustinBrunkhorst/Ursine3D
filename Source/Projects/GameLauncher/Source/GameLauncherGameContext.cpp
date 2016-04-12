#include "Precompiled.h"

#include "GameLauncherGameContext.h"

#include "GameLauncher.h"

#include <Application.h>

GameLauncherGameContext::GameLauncherGameContext(void)
    : GameContext( this )
    , m_launcher( GetCoreSystem( GameLauncher ) ) { }

bool GameLauncherGameContext::GetWindowFullScreen(void) const
{
    return m_launcher->GetWindow( )->IsFullScreen( );
}

void GameLauncherGameContext::SetWindowFullScreen(bool fullScreen)
{
    m_launcher->GetWindow( )->SetFullScreen( fullScreen );
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