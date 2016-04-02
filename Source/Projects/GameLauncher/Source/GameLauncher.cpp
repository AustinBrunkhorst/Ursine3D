/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Retrospect.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "GameLauncher.h"

#include <Application.h>

#include <WindowManager.h>
#include <UIManager.h>
#include <ScreenManager.h>
#include <Timer.h>

#include <Color.h>
#include <Vec3.h>
#include <CameraComponent.h>
#include <LightComponent.h>
#include <WorldSerializer.h>

#include <AudioManager.h>

using namespace ursine;

namespace
{
    const auto kEntryPoint = "file:///Assets/UI/Resources/Main.html";
}

CORE_SYSTEM_DEFINITION( GameLauncher );

GameLauncher::GameLauncher(void)
    : m_graphics( nullptr )
	, m_audioManager( nullptr )
    , m_mainWindow( { nullptr } )
{

}

GameLauncher::~GameLauncher(void)
{

}

Window::Handle GameLauncher::GetMainWindowHandle(void) const
{
    return m_mainWindow.window;
}

void GameLauncher::OnInitialize(void)
{
    
}

void GameLauncher::OnRemove(void)
{
    Application::Instance->Disconnect(
        APP_UPDATE,
        this,
        &GameLauncher::onAppUpdate
    );

    delete m_screenManager;

    m_screenManager = nullptr;

    m_mainWindow.window->Listener( this )
        .Off( WINDOW_FOCUS_CHANGED, &GameLauncher::onMainWindowFocusChanged )
        .Off( WINDOW_RESIZE, &GameLauncher::onMainWindowResize );

    m_mainWindow.ui->Close( );
    m_mainWindow.ui = nullptr;
    
    m_mainWindow.window = nullptr;

	m_audioManager = nullptr;
}

void GameLauncher::initializeGraphics(void)
{
    graphics::GfxConfig config;

    m_graphics->StartGraphics( config );
}

void GameLauncher::onAppUpdate(EVENT_HANDLER(ursine::Application))
{
    EVENT_ATTRS(Application, EventArgs);

    auto dt = sender->GetDeltaTime( );

    m_screenManager->Update( );

    SDL_ShowCursor( false );
}

void GameLauncher::onMainWindowFocusChanged(EVENT_HANDLER(ursine::Window))
{
    EVENT_ATTRS(Window, WindowFocusArgs);

    if (args->focused)
    {
        AudioManager::ResumeAudio( );
    }
    else 
    {
        AudioManager::PauseAudio( );
    }

    Application::Instance->SetActive( args->focused );
}

void GameLauncher::onMainWindowResize(EVENT_HANDLER(ursine::Window))
{
    EVENT_ATTRS(Window, WindowResizeArgs);

    m_graphics->Resize( args->width, args->height );
    m_mainWindow.viewport->SetDimensions( args->width, args->height );

    m_mainWindow.ui->SetViewport( {
        0, 0,
        args->width, args->height
    } );
}
