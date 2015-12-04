#include "Precompiled.h"

#include "Retrospect.h"

#include <Application.h>

#include <WindowManager.h>
#include <UIManager.h>
#include <ScreenManager.h>

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

    const auto kDefaultWindowWidth = 1280;
    const auto kDefaultWindowHeight = 720;
}

JSFunction(InitGame)
{
    gScreenManager->AddOverlay( "SplashScreen" );

    return CefV8Value::CreateUndefined( );
}

CORE_SYSTEM_DEFINITION( Retrospect );

Retrospect::Retrospect(void)
    : m_graphics( nullptr )
    , m_screenManager( nullptr )
    , m_mainWindow( { nullptr } )
{

}

Retrospect::~Retrospect(void)
{

}

void Retrospect::OnInitialize(void)
{
    auto *app = Application::Instance;

    app->Connect( APP_UPDATE, this, &Retrospect::onAppUpdate );

    auto *windowManager = GetCoreSystem( WindowManager );
    auto *uiManager = GetCoreSystem( UIManager );

    m_mainWindow.window = windowManager->AddWindow(
        "Retrospect",
        { 0, 0 },
        { static_cast<float>( kDefaultWindowWidth ), static_cast<float>( kDefaultWindowHeight ) },
        SDL_WINDOW_RESIZABLE
    );

    m_mainWindow.window->Listener( this )
        .On( WINDOW_RESIZE, &Retrospect::onMainWindowResize );

    m_mainWindow.window->SetLocationCentered( );

    m_graphics = GetCoreSystem( graphics::GfxAPI );

    initializeGraphics( );

    m_mainWindow.ui = uiManager->CreateView( m_mainWindow.window, kEntryPoint );

    m_mainWindow.ui->SetViewport( {
        0, 0,
        kDefaultWindowWidth, kDefaultWindowHeight
    } );

    m_screenManager = new ScreenManager( );
    m_screenManager->SetUI( m_mainWindow.ui );

    {
        SDL_DisplayMode displayMode;

        SDL_GetDesktopDisplayMode( 
            m_mainWindow.window->GetDisplayIndex( ),
            &displayMode 
        );

        m_mainWindow.window->SetSize( { 
            static_cast<float>( displayMode.w ), 
            static_cast<float>( displayMode.h ) 
        } );
    }
    m_mainWindow.window->SetFullScreen( true );
    m_mainWindow.window->Show( true );
}

void Retrospect::OnRemove(void)
{
    Application::Instance->Disconnect(
        APP_UPDATE,
        this,
        &Retrospect::onAppUpdate
    );

    delete m_screenManager;

    m_screenManager = nullptr;

    m_mainWindow.window->Listener( this )
        .Off( WINDOW_RESIZE, &Retrospect::onMainWindowResize );

    m_mainWindow.ui->Close( );
    m_mainWindow.ui = nullptr;
    
    m_mainWindow.window = nullptr;
}

void Retrospect::initializeGraphics(void)
{
    graphics::GfxConfig config;

    config.Fullscreen_ = false;

    config.HandleToWindow_ =
        static_cast<HWND>( m_mainWindow.window->GetPlatformHandle( ) );

    config.ModelListPath_ = "Assets/Models/";
    config.ShaderListPath_ = URSINE_SHADER_BUILD_DIRECTORY;
    config.TextureListPath_ = "Assets/Textures/";
    config.WindowWidth_ = kDefaultWindowWidth;
    config.WindowHeight_ = kDefaultWindowHeight;

    URSINE_TODO( "..." );

    config.m_renderUI = true;
    config.debug = false;

    config.Profile_ = false;

    m_graphics->StartGraphics( config );
    m_graphics->Resize( kDefaultWindowWidth, kDefaultWindowHeight );

    auto handle = m_graphics->ViewportMgr.CreateViewport( kDefaultWindowWidth, kDefaultWindowHeight );

    m_mainWindow.viewport = &m_graphics->ViewportMgr.GetViewport( handle );

    m_mainWindow.viewport->SetPosition( 0, 0 );
    m_mainWindow.viewport->SetBackgroundColor( 255.0f, 0.0f, 0.0f, 1.0f );

    m_graphics->SetGameViewport( handle );
}

void Retrospect::onAppUpdate(EVENT_HANDLER(ursine::Application))
{
    EVENT_ATTRS(Application, EventArgs);

    auto dt = sender->GetDeltaTime( );

    m_screenManager->Update( );
}

void Retrospect::onMainWindowResize(EVENT_HANDLER(ursine::Window))
{
    EVENT_ATTRS(Window, WindowResizeArgs);

    m_graphics->Resize( args->width, args->height );
    m_mainWindow.viewport->SetDimensions( args->width, args->height );

    m_mainWindow.ui->SetViewport( {
        0, 0,
        args->width, args->height
    } );
}
