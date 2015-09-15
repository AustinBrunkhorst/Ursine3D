#include "Precompiled.h"

#include "GraphicsTest.h"

#include <Application.h>

#include <WindowManager.h>
#include <UIManager.h>
#include <GfxAPI.h>

#include <Color.h>

namespace
{
    //const auto kGraphicsTestUIEntryPoint = "file:///Assets/Test.html";
    const auto kGraphicsTestUIEntryPoint = "www.google.com";

    const auto kDefaultWindowWidth = 1280;
    const auto kDefaultWindowHeight = 720;

    void onResize(int width, int height)
    {
        // ::OPENGL
        /*glMatrixMode( GL_PROJECTION );
        glLoadIdentity( );

        glViewport( 0, 0, width, height );
        glOrtho( 0, width, 0, height, -100.0f, 100.0f );*/

        //ursine::Application::Instance->GetCoreSystem<ursine::GfxAPI>( )->Resize( width, height );
    }
}

CORE_SYSTEM_DEFINITION( GraphicsTest );

GraphicsTest::GraphicsTest(void)
    : m_mainWindow( nullptr )
{
    
}

GraphicsTest::~GraphicsTest(void)
{
    // ::OPENGL
    //SDL_GL_DeleteContext( m_glContext );

    delete m_mainWindow;
}

void GraphicsTest::OnInitialize(void)
{
    auto *app = ursine::Application::Instance;

    app->Connect(
        ursine::APP_UPDATE, 
        this, 
        &GraphicsTest::onAppUpdate 
    );

    auto *windowManager = app->GetCoreSystem<ursine::WindowManager>( );
    auto *uiManager = app->GetCoreSystem<ursine::UIManager>( );

    m_mainWindow = windowManager->AddWindow(
        "Ursine3D Editor", 
        { 0, 0 }, 
        { static_cast<float>( kDefaultWindowWidth ), static_cast<float>( kDefaultWindowHeight ) },
        SDL_WINDOW_RESIZABLE
    );

    m_mainWindow->Listener( this )
        .On( ursine::WINDOW_RESIZE, &GraphicsTest::onMainWindowResize );
  
    m_mainWindow->SetLocationCentered( );
    m_mainWindow->Show( true );

    /////////////////////////////////////////////////////////////////
    //make graphics manager
    auto *gfxManager = app->GetCoreSystem<ursine::GfxAPI>( );

    HWND handle = reinterpret_cast<HWND>((m_mainWindow->GetPlatformHandle( )));

    ursine::GfxConfig config;
    config.Fullscreen_ = false;
    config.HandleToWindow_ = &handle;
    config.ModelListPath_ = "Models/";
    config.ShaderListPath_ = "SHADER_BINARY/";
    config.TextureListPath_ = "Textures/";
    config.WindowWidth_ = kDefaultWindowWidth;
    config.WindowHeight_ = kDefaultWindowHeight;
    config.m_renderUI = true;
    config.Profile_ = false;

    gfxManager->StartGraphics( config );

    m_ui = uiManager->CreateView( m_mainWindow, kGraphicsTestUIEntryPoint );

    m_ui->SetViewport( {
        0, 0,
        kDefaultWindowWidth, kDefaultWindowHeight
    } );

    onResize( kDefaultWindowWidth, kDefaultWindowHeight );
}

void GraphicsTest::OnRemove(void)
{
    ursine::Application::Instance->Disconnect(
        ursine::APP_UPDATE, 
        this, 
        &GraphicsTest::onAppUpdate 
    );

    m_mainWindow->Listener( this )
        .Off( ursine::WINDOW_RESIZE, &GraphicsTest::onMainWindowResize );

    m_ui->Close( );
}

void GraphicsTest::onAppUpdate(EVENT_HANDLER(ursine::Application))
{
    EVENT_ATTRS(ursine::Application, ursine::EventArgs);

    // ::OPENGL
    //glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // @@@ TODO:
    // Update Scene
    // Render Tools
    // Render Editor UIView

    auto gfx = ursine::Application::Instance->GetCoreSystem<ursine::GfxAPI>( );

    gfx->BeginScene( );
    gfx->EndScene( );

    m_ui->Draw( );

    // ::OPENGL
    //SDL_GL_SwapWindow( m_mainWindow->GetHandle( ) );
}

void GraphicsTest::onMainWindowResize(EVENT_HANDLER(ursine::Window))
{
    EVENT_ATTRS(ursine::Window, ursine::WindowResizeArgs);

    onResize( args->width, args->height );

    m_ui->SetViewport( {
        0, 0,
        args->width, args->height
    } );
}