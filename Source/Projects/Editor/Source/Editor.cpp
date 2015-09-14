#include "Precompiled.h"

#include "Editor.h"

#include <Application.h>

#include <WindowManager.h>
#include <UIManager.h>

#include <Color.h>

#include <GL/glew.h>

namespace
{
    const auto kEditorEntryPoint = "file:///Assets/Editor/Editor.html";
    const auto kEditorClearColor = ursine::Color( 0xFF252526 );

    const auto kDefaultWindowWidth = 1280;
    const auto kDefaultWindowHeight = 720;

    void onResize(int width, int height)
    {
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity( );

        glViewport( 0, 0, width, height );
        glOrtho( 0, width, 0, height, -100.0f, 100.0f );
    }
}

CORE_SYSTEM_DEFINITION( Editor );

Editor::Editor(void)
    : m_mainWindow( nullptr )
    , m_glContext( nullptr )
    , m_project( new Project( ) )
{
    
}

Editor::~Editor(void)
{
    URSINE_TODO( "temporary OpenGL" );
    SDL_GL_DeleteContext( m_glContext );

    delete m_mainWindow;

    delete m_project;
}

void Editor::OnInitialize(void)
{
    auto *app = ursine::Application::Instance;

    app->Connect(
        ursine::APP_UPDATE, 
        this, 
        &Editor::onAppUpdate 
    );

    auto *windowManager = app->GetCoreSystem<ursine::WindowManager>( );
    auto *uiManager = app->GetCoreSystem<ursine::UIManager>( );

    m_mainWindow = windowManager->AddWindow(
        "Ursine3D Editor", 
        { 0, 0 }, 
        { static_cast<float>( kDefaultWindowWidth ), static_cast<float>( kDefaultWindowHeight ) },
        URSINE_TODO( "temporary OpenGL" )
        SDL_WINDOW_OPENGL | 
        SDL_WINDOW_RESIZABLE
    );

    m_mainWindow->Listener( this )
        .On( ursine::WINDOW_RESIZE, &Editor::onMainWindowResize );
  
    m_mainWindow->SetLocationCentered( );
    m_mainWindow->Show( true );
    m_mainWindow->SetIcon( "Assets/Resources/Icon.png" );

    URSINE_TODO( "temporary OpenGL" );
    {
        // create an OpenGL context for this window
        m_glContext = SDL_GL_CreateContext( m_mainWindow->GetHandle( ) );

        glewExperimental = GL_TRUE;

        glewInit( );

        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        glClearColor( kEditorClearColor.r, kEditorClearColor.g, kEditorClearColor.b, kEditorClearColor.a );
    }

    m_ui = uiManager->CreateView( m_mainWindow, kEditorEntryPoint );

    m_ui->SetViewport( {
        0, 0,
        kDefaultWindowWidth, kDefaultWindowHeight
    } );

    onResize( kDefaultWindowWidth, kDefaultWindowHeight );
}

void Editor::OnRemove(void)
{
    ursine::Application::Instance->Disconnect(
        ursine::APP_UPDATE, 
        this, 
        &Editor::onAppUpdate 
    );

    m_mainWindow->Listener( this )
        .Off( ursine::WINDOW_RESIZE, &Editor::onMainWindowResize );

    m_ui->Close( );
}

void Editor::initializeTools(void)
{
    // @@@ TODO:
}

void Editor::onAppUpdate(EVENT_HANDLER(ursine::Application))
{
    EVENT_ATTRS(ursine::Application, ursine::EventArgs);

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // @@@ TODO:
    // Update Scene
    // Render Tools
    // Render Editor UIView

    m_ui->Draw( );

    SDL_GL_SwapWindow( m_mainWindow->GetHandle( ) );
}

void Editor::onMainWindowResize(EVENT_HANDLER(ursine::Window))
{
    EVENT_ATTRS(ursine::Window, ursine::WindowResizeArgs);

    onResize( args->width, args->height );

    m_ui->SetViewport( {
        0, 0,
        args->width, args->height
    } );
}