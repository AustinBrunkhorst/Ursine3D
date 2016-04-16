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
#include <UIResourceHandler.h>
#include <Timer.h>

using namespace ursine;

namespace
{
    const auto kUIEntryPoint = "file:///Resources/Content/Launcher.html";

    const auto kGameSettingsFile = "Resources/Launcher.config";
    const auto kGameIconFile = "Resources/Icon.png";
    const auto kGameResourcesPath = "Resources/Content";

    class UIResourceHandlerFactory : public CefSchemeHandlerFactory
    {
        CefRefPtr<CefResourceHandler> Create(
            CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            const CefString &schemeName,
            CefRefPtr<CefRequest> request
            ) override
        {
            auto *launcher = GetCoreSystem( GameLauncher );

            return new UIResourceHandler(
                kUIGameResourceDomain,
                &launcher->GetScene( )->GetResourceManager( )
            );
        }

        IMPLEMENT_REFCOUNTING(UIResourceHandlerFactory);
    };
}

namespace ursine
{
    UIScreenManager *JSGetGlobalScreenManager(void)
    {
        return &GetCoreSystem( GameLauncher )->GetScene( )->GetScreenManager( );
    }

    CefRefPtr<CefBrowser> JSGetGlobalBrowser(void)
    {
        return GetCoreSystem( GameLauncher )->GetUI( )->GetBrowser( );
    }
}

CORE_SYSTEM_DEFINITION( GameLauncher );

GameLauncher::GameLauncher(void)
    : m_graphics( nullptr )
    , m_scene( nullptr )
    , m_gameContext( nullptr )
    , m_window( { nullptr } ) { }


GameLauncher::GameLauncher(const GameLauncher &rhs)
    : m_graphics( nullptr )
    , m_scene( nullptr )
    , m_gameContext( nullptr )
    , m_window( { nullptr } ) { }

GameLauncher::~GameLauncher(void)
{

}

Scene *GameLauncher::GetScene(void)
{
    return m_scene;
}

Window::Handle GameLauncher::GetWindow(void)
{
    return m_window.window;
}

UIView::Handle GameLauncher::GetUI(void)
{
    return m_window.ui;
}

void GameLauncher::OnInitialize(void)
{
    m_scene = new Scene( );
    m_scene->GetResourceManager( ).SetResourceDirectory( kGameResourcesPath );

    m_gameContext = new GameLauncherGameContext( m_scene );

    m_scene->SetGameContext( m_gameContext );

    initSettings( );
    initWindow( );
    initGraphics( );
    initUI( );

    if (m_settings.windowFullScreen)
    {
        // make sure we have time to switch into fullscreen
        Application::PostMainThread( std::bind( &GameLauncher::initBuildSettings, this ) );
    }
    else
    {
        initBuildSettings( );
    }

    Application::Instance->Connect(
        APP_UPDATE,
        this,
        &GameLauncher::onAppUpdate
    );
}

void GameLauncher::OnRemove(void)
{
    delete m_gameContext->GetManager( );

    m_gameContext->SetManager( nullptr );

    m_scene->SetGameContext( nullptr );

    delete m_gameContext;

    delete m_scene;

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

    ursine::uint32 flags = 0;

    if (m_settings.windowResizable)
        utils::FlagSet( flags, SDL_WINDOW_RESIZABLE );

    auto window = m_window.window = windowManager->AddWindow(
        m_settings.windowTitle,
        Vec2::Zero( ),
        kDefaultWindowDimensions,
        flags
    );

    if (m_settings.windowFullScreen)
    {
        SDL_DisplayMode fullScreenMode;

        SDL_GetDesktopDisplayMode( window->GetDisplayIndex( ), &fullScreenMode );

        window->SetSize( {
            static_cast<float>( fullScreenMode.w ),
            static_cast<float>( fullScreenMode.h )
        } );

        window->SetFullScreen( true );
    }

    window->SetLocationCentered( );

    if (fs::exists( kGameIconFile ))
        window->SetIcon( kGameIconFile );

    SDL_ShowCursor( false );

    window->Show( true );

    window->Listener( this )
        .On( WINDOW_RESIZE, &GameLauncher::onWindowResize )
        .On( WINDOW_FOCUS_CHANGED, &GameLauncher::onWindowFocusChanged );
}

void GameLauncher::initGraphics(void)
{
    m_graphics = GetCoreSystem( graphics::GfxAPI );

    graphics::GfxConfig gfxConfig;

    gfxConfig.fullscreen = false;// m_settings.windowFullScreen;
      
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
      
    auto viewport = m_graphics->ViewportMgr.CreateViewport( 
        gfxConfig.windowWidth, 
        gfxConfig.windowHeight 
    );
       
    auto &handle = m_graphics->ViewportMgr.GetViewport( viewport );
        
    handle.SetPosition( 0, 0 );
      
    m_scene->SetViewport( viewport );
     
    m_graphics->SetGameViewport( viewport );

    m_window.viewport = &handle;
}

void GameLauncher::initUI(void)
{
    auto *uiManager = GetCoreSystem( UIManager );

    auto window = m_window.window;
    auto &size = window->GetSize( );

    // create the UI resource factory
    CefRegisterSchemeHandlerFactory( 
        "http", 
        kUIGameResourceDomain, 
        new UIResourceHandlerFactory( ) 
    );

    m_window.ui = uiManager->CreateView( 
        window, 
        kUIEntryPoint 
    );

    m_window.ui->SetViewport( {
        0, 0,
        static_cast<int>( size.X( ) ), 
        static_cast<int>( size.Y( ) )
    } );

    m_scene->GetScreenManager( ).SetUI( m_window.ui );
}

void GameLauncher::initBuildSettings(void)
{
    // make sure components are configured
    ecs::EntityManager::ConfigureComponents( );

    auto managerType = meta::Type::GetFromName( m_settings.manager );

    UAssert( managerType.IsValid( ) && managerType.GetBaseClasses( ).count( typeof( GameManager ) ),
        "Unknown or invalid Game Manager '%s'.",
        m_settings.manager.c_str( )
    );

    auto ctor = managerType.GetDynamicConstructor( );

    UAssert( ctor.IsValid( ), 
        "Game Manager '%s' missing default constructor." 
    );

    auto *manager = ctor.Invoke( ).GetValue<GameManager*>( );

    m_gameContext->SetManager( manager );

    manager->OnInitialize( 
        m_gameContext, 
        m_settings.managerConfiguration 
    );
}

void GameLauncher::onAppUpdate(EVENT_HANDLER(ursine::Application))
{
    EVENT_ATTRS(Application, EventArgs);

    m_scene->Update( sender->GetDeltaTime( ) );
    m_scene->Render( );

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

    GameContextWindowFocusArgs e( args->focused );

    m_gameContext->Dispatch( GC_WINDOW_FOCUS_CHANGED, &e );
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
