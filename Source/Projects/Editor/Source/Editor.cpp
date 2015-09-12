#include "Precompiled.h"

#include "Editor.h"

#include <Application.h>

#include <WindowManager.h>
#include <UIManager.h>

#include <GL/glew.h>

namespace
{
    const int kDefaultWindowWidth = 1280;
    const int kDefaultWindowHeight = 720;
}

CORE_SYSTEM_DEFINITION( Editor );

Editor::Editor(void)
    : m_mainWindow( nullptr )
    , m_glContext( nullptr )
{
    ursine::Application::Instance->Connect(
        ursine::APP_UPDATE, 
        this, 
        &Editor::onAppUpdate 
    );
}

Editor::~Editor(void)
{
    ursine::Application::Instance->Disconnect(
        ursine::APP_UPDATE, 
        this, 
        &Editor::onAppUpdate 
    );

    m_mainWindow->Listener( this )
        .Off( ursine::WINDOW_RESIZE, &Editor::onMainWindowResize );

    URSINE_TODO( "temporary OpenGL" );
    SDL_GL_DeleteContext( m_glContext );

    delete m_mainWindow;
}

void Editor::OnInitialize(void)
{
    auto *app = ursine::Application::Instance;

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

    URSINE_TODO( "temporary OpenGL" );
    {
        // create an OpenGL context for this window
        m_glContext = SDL_GL_CreateContext( m_mainWindow->GetHandle( ) );

        glewExperimental = GL_TRUE;

        glewInit( );
    }

    m_ui = uiManager->CreateView( m_mainWindow, "http://www.google.com/" );

    m_ui->SetViewport( {
        0, 0,
        kDefaultWindowWidth, kDefaultWindowHeight
    } );
}

void Editor::initializeTools(void)
{
    // @@@ TODO:
}

void Editor::onAppUpdate(EVENT_HANDLER(ursine::Application))
{
    EVENT_ATTRS(ursine::Application, ursine::EventArgs);

    glClear( GL_COLOR_BUFFER_BIT );

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

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );

    glViewport( 0, 0, args->width, args->height );
    glOrtho( -1, 1, -1, 1, -100.0f, 100.0f );

    m_ui->SetViewport( {
        0, 0,
        args->width, args->height
    } );
}