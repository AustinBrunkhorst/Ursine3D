/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Editor.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/
 
#include "Precompiled.h"
 
#include "Editor.h"
#include "Project.h" 
 
#include <Application.h>
   
#include <Scene.h>
#include <WindowManager.h>
#include <UIManager.h>

using namespace ursine;

namespace
{
    const auto kWindowTitle = "Ursine3D Editor";

    const std::string kEntryPointDir = "file:///Assets/UI/Resources/";

    const auto kEntryPointLauncher = "Launcher.html";
    const auto kEntryPointEditor = "Editor.html";
}

CORE_SYSTEM_DEFINITION( Editor );

Editor::Editor(void)
    : m_graphics( nullptr ) { }

Editor::~Editor(void) { }

///////////////////////////////////////////////////////////////////////////////
// Misc
///////////////////////////////////////////////////////////////////////////////

const EditorWindow &Editor::GetMainWindow(void) const
{
    return m_mainWindow;
}

///////////////////////////////////////////////////////////////////////////////

Project &Editor::GetProject(void)
{
    return m_project;
}

///////////////////////////////////////////////////////////////////////////////
// Notifications
///////////////////////////////////////////////////////////////////////////////

NotificationManager &Editor::GetNotificationManager(void)
{
    return m_notificationManager;
}

///////////////////////////////////////////////////////////////////////////////

Notification Editor::PostNotification(const NotificationConfig &config)
{
    return m_notificationManager.Create( config );
}

///////////////////////////////////////////////////////////////////////////////
// Core System
///////////////////////////////////////////////////////////////////////////////

void Editor::OnInitialize(void)
{
    auto *app = Application::Instance;

    auto argc = app->GetArgC( );
    auto argv = app->GetArgV( );

    m_startupConfig.windowSize = Vec2::One( ) * 600.0f;

    // no project file, open launcher
    if (argc == 1)
    {
        m_startupConfig.uiEntryPoint = kEntryPointDir + kEntryPointLauncher;
        m_startupConfig.updateHandler = &Editor::onLauncherUpdate;
    }
    else
    {
        m_startupConfig.uiEntryPoint = kEntryPointDir + kEntryPointEditor;
        m_startupConfig.updateHandler = &Editor::onEditorUpdate;
    }

    startup( );
}

///////////////////////////////////////////////////////////////////////////////

void Editor::OnRemove(void)
{
    Application::Instance->Disconnect(
        APP_UPDATE,
        this,
        m_startupConfig.updateHandler
    );

    m_mainWindow.m_window->Listener( this )
        .Off( WINDOW_RESIZE, &Editor::onMainWindowResize ); 

    m_mainWindow.m_ui->Close( );
    m_mainWindow.m_ui = nullptr;

    m_mainWindow.m_window = nullptr;
}

///////////////////////////////////////////////////////////////////////////////
// Initialization
///////////////////////////////////////////////////////////////////////////////

void Editor::startup(void)
{
    Application::Instance->Connect(
        APP_UPDATE,
        this,
        m_startupConfig.updateHandler
    );
     
    initializeWindow( );
    initializeGraphics( );
    initializeUI( );
}

///////////////////////////////////////////////////////////////////////////////

void Editor::initializeWindow(void)
{
    auto *windowManager = GetCoreSystem( WindowManager );

    m_mainWindow.m_window = windowManager->AddWindow(
        kWindowTitle,
        Vec2::Zero( ),
        m_startupConfig.windowSize,
        SDL_WINDOW_RESIZABLE
    );

    m_mainWindow.m_window->Listener( this )
        .On( WINDOW_RESIZE, &Editor::onMainWindowResize );

    m_mainWindow.m_window->SetLocationCentered( );
    m_mainWindow.m_window->Show( true );
    m_mainWindow.m_window->SetIcon( "Assets/Resources/Icon.png" );
}

///////////////////////////////////////////////////////////////////////////////

void Editor::initializeGraphics(void)
{ 
    m_graphics = GetCoreSystem( graphics::GfxAPI );

    graphics::GfxConfig gfxConfig;

    gfxConfig.fullscreen = false;
      
    gfxConfig.handleToWindow =
        static_cast<HWND>( m_mainWindow.m_window->GetPlatformHandle( ) );
       
    gfxConfig.modelListPath = "Assets/Models/";
    gfxConfig.shaderListPath = URSINE_SHADER_BUILD_DIRECTORY;
    gfxConfig.textureListPath = "Assets/Textures/";
    gfxConfig.windowWidth = static_cast<unsigned>( m_startupConfig.windowSize.X( ) );
    gfxConfig.windowHeight = static_cast<unsigned>( m_startupConfig.windowSize.Y( ) );
    gfxConfig.enableDebugInfo = false;
    gfxConfig.enableProfiling = false;
        
    m_graphics->StartGraphics( gfxConfig );
    m_graphics->Resize( gfxConfig.windowWidth, gfxConfig.windowHeight );

    auto viewport = m_graphics->ViewportMgr.CreateViewport( 0, 0 );

    auto &handle = m_graphics->ViewportMgr.GetViewport( viewport );

    handle.SetPosition( 0, 0 );

    m_project.GetScene( ).SetViewport( viewport );

    m_graphics->SetGameViewport( viewport );
}

///////////////////////////////////////////////////////////////////////////////

void Editor::initializeUI(void)
{
    auto *uiManager = GetCoreSystem( UIManager );

    m_mainWindow.m_ui = uiManager->CreateView( 
        m_mainWindow.m_window, 
        m_startupConfig.uiEntryPoint 
    );

    m_mainWindow.m_ui->SetViewport( {
        0, 0,
        static_cast<int>( m_startupConfig.windowSize.X( ) ), 
        static_cast<int>( m_startupConfig.windowSize.Y( ) )
    } );

    m_notificationManager.SetUI( m_mainWindow.m_ui );
}

///////////////////////////////////////////////////////////////////////////////
// Event Handlers
///////////////////////////////////////////////////////////////////////////////

void Editor::onLauncherUpdate(EVENT_HANDLER(Application))
{
    m_mainWindow.m_ui->DrawMain( );
}

///////////////////////////////////////////////////////////////////////////////

void Editor::onEditorUpdate(EVENT_HANDLER(Application))
{
    EVENT_ATTRS(Application, EventArgs);

    auto dt = sender->GetDeltaTime( );

    auto &scene = m_project.GetScene( );

    scene.Update( dt );
    scene.Render( );

    m_mainWindow.m_ui->DrawMain( );
}

///////////////////////////////////////////////////////////////////////////////

void Editor::onMainWindowResize(EVENT_HANDLER(Window))
{
    EVENT_ATTRS(Window, WindowResizeArgs);

    m_graphics->Resize( args->width, args->height );

    m_mainWindow.m_ui->SetViewport( {
        0, 0,
        args->width, args->height
    } );
}
