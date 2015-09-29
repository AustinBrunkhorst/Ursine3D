#include "Precompiled.h"

#include "Editor.h"
#include "Project.h"

#include <Application.h>

#include <WindowManager.h>
#include <UIManager.h>

#include <Color.h>

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
    , m_project( new Project( ) )
{
    auto entity = m_project->GetScene( ).GetWorld( ).CreateEntity( );
}

Editor::~Editor(void)
{
    delete m_mainWindow.window;

    delete m_project;
}

void Editor::OnInitialize(void)
{
    auto *app = Application::Instance;

    app->Connect(
        APP_UPDATE, 
        this, 
        &Editor::onAppUpdate 
    );

    auto *windowManager = app->GetCoreSystem<WindowManager>( );
    auto *uiManager = app->GetCoreSystem<UIManager>( );

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

    m_graphics = app->GetCoreSystem<GfxAPI>( );

    initializeGraphics( );

    m_mainWindow.ui = uiManager->CreateView( m_mainWindow.window, kEditorEntryPoint );

    m_mainWindow.ui->SetViewport( {
        0, 0,
        kDefaultWindowWidth, kDefaultWindowHeight
    } );

    resizeMainWindow( kDefaultWindowWidth, kDefaultWindowHeight );
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

    m_mainWindow.viewport = m_graphics->ViewportMgr.CreateViewport(
        kDefaultWindowWidth, 
        kDefaultWindowHeight 
    );

    m_mainWindow.camera = m_graphics->CameraMgr.AddCamera( );

    auto &viewportHandle = m_graphics->ViewportMgr.GetViewport( m_mainWindow.viewport );

    viewportHandle.SetRenderMode( VIEWPORT_RENDER_FORWARD );
    viewportHandle.SetViewportCamera( m_mainWindow.camera );
}

void Editor::initializeTools(void)
{
    // @@@ TODO:
}

void Editor::resizeMainWindow(int width, int height)
{
    auto &viewportHandle = m_graphics->ViewportMgr.GetViewport( m_mainWindow.viewport );

    viewportHandle.SetDimensions( width, height );
}

void Editor::onAppUpdate(EVENT_HANDLER(Application))
{
    EVENT_ATTRS(Application, EventArgs);

    m_graphics->BeginScene( );

    // @@@ TODO:
    // Update Scene
    // Render Tools
    // Render Editor UIView

    m_graphics->RenderScene( sender->GetDeltaTime( ), m_mainWindow.viewport );
    m_graphics->EndScene( );
}

void Editor::onMainWindowResize(EVENT_HANDLER(Window))
{
    EVENT_ATTRS(Window, WindowResizeArgs);

    resizeMainWindow( args->width, args->height );

    m_mainWindow.ui->SetViewport( {
        0, 0,
        args->width, args->height
    } );

    m_graphics->Resize( args->width, args->height );
}