#include "Precompiled.h"

#include "Retrospect.h"

#include <Application.h>

#include <WindowManager.h>
#include <UIManager.h>

#include <Color.h>
#include <Vec3.h>
#include <CameraComponent.h>
#include <LightComponent.h>
#include <WorldSerializer.h>

using namespace ursine;

namespace
{
    const auto kEntryPoint = "";

    const auto  kStartWorld = "Assets/Worlds/SubmissionSemester1.uworld";

    const auto kClearColor = Color(0xFF252526);

    const auto kDefaultWindowWidth = 1280;
    const auto kDefaultWindowHeight = 720;
}

CORE_SYSTEM_DEFINITION( Retrospect );

Retrospect::Retrospect(void)
    : m_graphics( nullptr )
    , m_mainWindow( { nullptr } )
    , m_scene( std::make_shared<Scene>( ) )
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
        "Ursine3D Editor",
        { 0, 0 },
        { static_cast<float>( kDefaultWindowWidth ), static_cast<float>( kDefaultWindowHeight ) },
        SDL_WINDOW_RESIZABLE
    );

    m_mainWindow.window->Listener( this )
        .On( WINDOW_RESIZE, &Retrospect::onMainWindowResize );

    m_mainWindow.window->SetLocationCentered( );
    m_mainWindow.window->Show( true );
    m_mainWindow.window->SetIcon( "Assets/Resources/Icon.png" );

    m_graphics = GetCoreSystem( graphics::GfxAPI );

    initializeGraphics( );

    // m_mainWindow.ui = uiManager->CreateView( m_mainWindow.window, kEntryPoint );

    /*m_mainWindow.ui->SetViewport( {
        0, 0,
        kDefaultWindowWidth, kDefaultWindowHeight
    } );*/

    initializeScene( );
}

void Retrospect::OnRemove(void)
{
    Application::Instance->Disconnect(
        APP_UPDATE,
        this,
        &Retrospect::onAppUpdate
    );

    m_mainWindow.window->Listener( this )
        .Off( WINDOW_RESIZE, &Retrospect::onMainWindowResize );

    //m_mainWindow.ui->Close( );
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
}

void Retrospect::initializeScene(void)
{
    auto world = m_scene->GetWorld( );
    {
        auto viewport = m_graphics->ViewportMgr.CreateViewport( kDefaultWindowWidth, kDefaultWindowHeight );

        auto &handle = m_graphics->ViewportMgr.GetViewport( viewport );

        handle.SetPosition( 0, 0 );

        handle.SetBackgroundColor( 255.0f, 0.0f, 0.0f, 1.0f );

        m_scene->SetViewport( viewport );

        m_graphics->SetGameViewport( viewport );

        ecs::WorldSerializer serializer;

        serializer.Deserialize( kStartWorld, world );

        m_scene->SetWorld( world );
    }

    world->DispatchLoad( );
}

void Retrospect::onAppUpdate(EVENT_HANDLER(ursine::Application))
{
    EVENT_ATTRS(Application, EventArgs);

    auto dt = sender->GetDeltaTime( );

    m_scene->Update( dt );

    m_graphics->StartFrame( );  

    m_scene->Render( );

    //m_mainWindow.ui->DrawMain( );  

    m_graphics->EndFrame( );
}

void Retrospect::onMainWindowResize(EVENT_HANDLER(ursine::Window))
{
    EVENT_ATTRS(Window, WindowResizeArgs);

    m_graphics->Resize( args->width, args->height );

    m_mainWindow.ui->SetViewport( {
        0, 0,
        args->width, args->height
    } );
}
