#include "Precompiled.h"

#include "Editor.h"
#include "Project.h"

#include <Application.h>

#include <WindowManager.h>
#include <UIManager.h>

#include <Color.h>

#include <CameraComponent.h>
#include <RenderableComponent.h>

using namespace ursine;

namespace
{
    const auto kEditorEntryPoint = "file:///Assets/UI/Resources/Main.html";
    const auto kEditorClearColor = Color( 0xFF252526 );

    const auto kDefaultWindowWidth = 1280;
    const auto kDefaultWindowHeight = 720;
}

CORE_SYSTEM_DEFINITION( Editor );

Editor::Editor(void)
    : m_graphics( nullptr )
    , m_mainWindow( { nullptr } )
    , m_project( nullptr )
{

}

Editor::~Editor(void)
{

}

void Editor::OnInitialize(void)
{
    auto *app = Application::Instance;

    app->Connect(
        APP_UPDATE, 
        this, 
        &Editor::onAppUpdate 
    );

    auto *windowManager = CoreSystem( WindowManager );
    auto *uiManager = CoreSystem( UIManager );

    m_mainWindow.window = windowManager->AddWindow(
        "Ursine3D Editor", 
        { 0, 0 }, 
        { static_cast<float>( kDefaultWindowWidth ), static_cast<float>( kDefaultWindowHeight ) },
        SDL_WINDOW_RESIZABLE
    );

    m_mainWindow.window->Listener( this )
        .On( WINDOW_RESIZE, &Editor::onMainWindowResize );
  
    m_mainWindow.window->SetLocationCentered( );
    m_mainWindow.window->Show( true );
    m_mainWindow.window->SetIcon( "Assets/Resources/Icon.png" );

    m_graphics = CoreSystem( GfxAPI );

    m_project = new Project( );

    initializeGraphics( );

    m_mainWindow.ui = uiManager->CreateView( m_mainWindow.window, kEditorEntryPoint );

    m_mainWindow.ui->SetViewport( {
        0, 0,
        kDefaultWindowWidth, kDefaultWindowHeight
    } );
}

void Editor::OnRemove(void)
{
    Application::Instance->Disconnect(
        APP_UPDATE, 
        this, 
        &Editor::onAppUpdate 
    );

    m_mainWindow.window->Listener( this )
        .Off( WINDOW_RESIZE, &Editor::onMainWindowResize );

    m_mainWindow.ui->Close( );

    delete m_mainWindow.window;

    m_mainWindow.window = nullptr;

    delete m_project;

    m_project = nullptr;
}

Project *Editor::GetProject(void) const
{
    return m_project;
}

void Editor::initializeGraphics(void)
{
    GfxConfig config;

    config.Fullscreen_ = false;

    config.HandleToWindow_ = 
        static_cast<HWND>( m_mainWindow.window->GetPlatformHandle( ) );

    config.ModelListPath_ = "Assets/Models/";
    config.ShaderListPath_ = "Assets/Shaders/";
    config.TextureListPath_ = "Assets/Textures/";
    config.WindowWidth_ = kDefaultWindowWidth;
    config.WindowHeight_ = kDefaultWindowHeight;

    URSINE_TODO( "..." );
    config.m_renderUI = true;

    config.Profile_ = false;

    m_graphics->StartGraphics( config );
    m_graphics->Resize( kDefaultWindowWidth, kDefaultWindowHeight );

    auto &scene = m_project->GetScene( );
    {
        auto viewport = m_graphics->ViewportMgr.CreateViewport(
            static_cast<int>( 0.85f * kDefaultWindowWidth ), 
            static_cast<int>( kDefaultWindowHeight - (30.0f + 27.0f) )
        );

        auto &handle = m_graphics->ViewportMgr.GetViewport( viewport );

        handle.SetPosition( 
            static_cast<int>( 0.15f * kDefaultWindowWidth ), 
            static_cast<int>( 30.0f + 27.0f ) 
        );

        scene.SetViewport( viewport );

        m_graphics->SetGameViewport( viewport );
    }

    auto &world = scene.GetWorld( );

    auto *cameraEntity = world.CreateEntity( );
    {
        auto &camera = cameraEntity->AddComponent<ecs::Camera>( )->GetCamera( );

        camera.SetPosition( 0.0f, 0.0f );
        camera.SetDimensions( 1.0f, 1.0f );

        camera.LookAtPoint( { 0.0f, 0.0f, 0.0f } );
    }

    auto *boxEntity = world.CreateEntity( );
    {
        auto boxHandle = m_graphics->RenderableMgr.AddRenderable( RENDERABLE_MODEL3D );

        auto &box = m_graphics->RenderableMgr.GetModel3D( boxHandle );

        box.SetModel( "Skeleton" );
        box.SetMaterial( "Blank" );

        SMat4 transform;

        transform.Translate( { 0.0f, 0.0f, 85.0f } );

        box.SetWorldMatrix( transform );

        auto *component = boxEntity->AddComponent<ecs::Renderable>( );

        component->SetHandle( boxHandle );
    }

    auto *directionLight = world.CreateEntity( );
    {
        auto lightHandle = m_graphics->RenderableMgr.AddRenderable( RENDERABLE_DIRECTION_LIGHT );

        auto &light = m_graphics->RenderableMgr.GetDirectionalLight( lightHandle );

        light.SetDirection( { 0.0f, 1.0f, 0.0f } );
        light.SetColor( Color::White );

        auto *component = directionLight->AddComponent<ecs::Renderable>( );

        component->SetHandle( lightHandle );
    }

    auto *pointLight = world.CreateEntity( );
    {
        auto lightHandle = m_graphics->RenderableMgr.AddRenderable( RENDERABLE_POINT_LIGHT );

        auto &light = m_graphics->RenderableMgr.GetPointLight( lightHandle );

        light.SetPosition( { 0.0f, 0.0f, 0.0f } );
        light.SetRadius( 100.0f );
        light.SetColor( Color::Red );

        auto *component = pointLight->AddComponent<ecs::Renderable>( );

        component->SetHandle( lightHandle );
    }
}

void Editor::initializeTools(void)
{
    // @@@ TODO:
}

void Editor::onAppUpdate(EVENT_HANDLER(Application))
{
    EVENT_ATTRS(Application, EventArgs);

    auto dt = sender->GetDeltaTime( );

    auto &scene = m_project->GetScene( );

    scene.Update( dt );

    m_graphics->StartFrame( );

    scene.Render( );

    m_mainWindow.ui->DrawMain( );

    m_graphics->EndFrame( );
}

void Editor::onMainWindowResize(EVENT_HANDLER(Window))
{
    EVENT_ATTRS(Window, WindowResizeArgs);

    m_mainWindow.ui->SetViewport( {
        0, 0,
        args->width, args->height
    } );

    m_graphics->Resize( args->width, args->height );
}