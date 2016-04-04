/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GameLauncher.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "GameLauncher.h"

#include <AudioManager.h>
#include <WindowManager.h>
#include <UIManager.h>

using namespace ursine;

namespace
{
    const auto kUIEntryPoint = "file:///Resources/UI/Main.html";

    const auto kGameSettingsFile = "Resources/Game.config";
    const auto kGameIconFile = "Resources/Icon.png";
    const auto kGameResourcesPath = "Resources/Content";

    const Vec2 kDefaultWindowDimensions { 1280, 720 };
}

namespace ursine
{
    UIScreenManager *JSGetGlobalScreenManager(void)
    {
        return &GetCoreSystem( GameLauncher )->GetScene( ).GetScreenManager( );
    }

    CefRefPtr<CefBrowser> JSGetGlobalBrowser(void)
    {
        return GetCoreSystem( GameLauncher )->GetUI( )->GetBrowser( );
    }
}

CORE_SYSTEM_DEFINITION( GameLauncher );

GameLauncher::GameLauncher(void)
    : m_graphics( nullptr )
    , m_window( { nullptr } ) { }


GameLauncher::GameLauncher(const GameLauncher &rhs)
    : m_graphics( nullptr )
    , m_window( { nullptr } ) { }

GameLauncher::~GameLauncher(void)
{

}

Scene &GameLauncher::GetScene(void)
{
    return m_scene;
}

UIView::Handle GameLauncher::GetUI(void)
{
    return m_window.ui;
}

void GameLauncher::OnInitialize(void)
{
    m_scene.GetResourceManager( ).SetResourceDirectory( kGameResourcesPath );

    //initSettings( );
    initWindow( );
    initGraphics( );
    initUI( );

    Application::Instance->Connect(
        APP_UPDATE,
        this,
        &GameLauncher::onAppUpdate
    );
}

void GameLauncher::OnRemove(void)
{
    Application::Instance->Disconnect(
        APP_UPDATE,
        this,
        &GameLauncher::onAppUpdate
    );

    m_window.window->Listener( this )
        .Off( WINDOW_FOCUS_CHANGED, &GameLauncher::onWindowFocusChanged )
        .Off( WINDOW_RESIZE, &GameLauncher::onWindowResize );

    m_window.ui->Close( );
    m_window.ui = nullptr;
    
    m_window.window = nullptr;
}

void GameLauncher::initSettings(void)
{
    std::string configJson;
    std::string configJsonError;

    UAssert( fs::LoadAllText( kGameSettingsFile, configJson ),
        "Unable to load game configuration file."
    );

    auto jsonData = Json::parse( configJson, configJsonError );

    UAssert( configJsonError.empty( ),
        "Error parsing game configuration JSON.\nerror: %s",
        configJsonError.c_str( )
    );

    m_settings = meta::Type::DeserializeJson<GameSettings>( jsonData );
}

void GameLauncher::initWindow(void)
{
    auto *windowManager = GetCoreSystem( WindowManager );

    auto window = m_window.window = windowManager->AddWindow(
        m_settings.title,
        Vec2::Zero( ),
        kDefaultWindowDimensions,
        0
    );

    window->Listener( this )
        .On( WINDOW_RESIZE, &GameLauncher::onWindowResize )
        .On( WINDOW_FOCUS_CHANGED, &GameLauncher::onWindowFocusChanged );
    
    window->SetLocationCentered( );

    if (fs::exists( kGameIconFile ))
        window->SetIcon( kGameIconFile );

    SDL_ShowCursor( false );

    window->Show( true );
}

void GameLauncher::initGraphics(void)
{
    m_graphics = GetCoreSystem( graphics::GfxAPI );

    graphics::GfxConfig gfxConfig;

    gfxConfig.fullscreen = false;
      
    auto window = m_window.window;
    auto &size = window->GetSize( );

    gfxConfig.handleToWindow =
        static_cast<HWND>( window->GetPlatformHandle( ) );
       
    gfxConfig.shaderListPath = URSINE_SHADER_BUILD_DIRECTORY;
    gfxConfig.windowWidth = static_cast<unsigned>( size.X( ) );
    gfxConfig.windowHeight = static_cast<unsigned>( size.Y( ) );
    gfxConfig.enableDebugInfo = false;
    gfxConfig.enableProfiling = false;

    m_graphics->StartGraphics( gfxConfig );
    m_graphics->Resize( gfxConfig.windowWidth, gfxConfig.windowHeight );
      
    auto viewport = m_graphics->ViewportMgr.CreateViewport( 0, 0 );
       
    auto &handle = m_graphics->ViewportMgr.GetViewport( viewport );
        
    handle.SetPosition( 0, 0 );
      
    m_scene.SetViewport( viewport );
     
    m_graphics->SetGameViewport( viewport );
}

void GameLauncher::initUI(void)
{
    auto *uiManager = GetCoreSystem( UIManager );

    auto window = m_window.window;
    auto &size = window->GetSize( );

    m_window.ui = uiManager->CreateView( 
        window, 
        kUIEntryPoint 
    );

    m_window.ui->SetViewport( {
        0, 0,
        static_cast<int>( size.X( ) ), 
        static_cast<int>( size.Y( ) )
    } );
}

void GameLauncher::onAppUpdate(EVENT_HANDLER(ursine::Application))
{
    EVENT_ATTRS(Application, EventArgs);

    m_window.ui->DrawMain( );
}

void GameLauncher::onWindowFocusChanged(EVENT_HANDLER(ursine::Window))
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

void GameLauncher::onWindowResize(EVENT_HANDLER(ursine::Window))
{
    EVENT_ATTRS(Window, WindowResizeArgs);

    m_graphics->Resize( args->width, args->height );
    m_window.viewport->SetDimensions( args->width, args->height );

    m_window.ui->SetViewport( {
        0, 0,
        args->width, args->height
    } );
}
