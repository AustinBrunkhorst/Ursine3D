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
    const auto kLauncherEntryPoint = "file:///Assets/UI/Resources/Launcher.html";
    const auto kEditorEntryPoint = "file:///Assets/UI/Resources/Editor.html";

    const Vec2 kLauncherWindowSize { 640, 480 };

    const auto kDefaultWindowWidth = 1280;
    const auto kDefaultWindowHeight = 720;
}

CORE_SYSTEM_DEFINITION( Editor );

Editor::Editor(void)
    : m_graphics( nullptr ) { }

Editor::~Editor(void) { }

void Editor::OnInitialize(void)
{
    auto *app = Application::Instance;

    app->Connect(
        APP_UPDATE,
        this,
        &Editor::onAppUpdate 
    );
     
    auto *windowManager = GetCoreSystem( WindowManager );

    m_mainWindow.m_window = windowManager->AddWindow(
        "Ursine3D Editor",
        { 0, 0 },
        { static_cast<float>( kDefaultWindowWidth ), static_cast<float>( kDefaultWindowHeight ) },
        SDL_WINDOW_RESIZABLE
    );

    m_mainWindow.m_window->Listener( this )
        .On( WINDOW_RESIZE, &Editor::onMainWindowResize );

    m_mainWindow.m_window->SetLocationCentered( );
    m_mainWindow.m_window->Show( true );
    m_mainWindow.m_window->SetIcon( "Assets/Resources/Icon.png" );

    m_graphics = GetCoreSystem( graphics::GfxAPI );

    initializeGraphics( );

    auto viewport = m_graphics->ViewportMgr.CreateViewport( 0, 0 );

    auto &handle = m_graphics->ViewportMgr.GetViewport( viewport );

    handle.SetPosition( 0, 0 );

    handle.SetBackgroundColor( 255.0f, 0.0f, 0.0f, 1.0f );

    m_project.GetScene( ).SetViewport( viewport );

    m_graphics->SetGameViewport( viewport );

    initializeUI( );

    m_mainWindow.m_ui->SetViewport( {
        0, 0,
        kDefaultWindowWidth, kDefaultWindowHeight
    } );

    m_notificationManager.SetUI( m_mainWindow.m_ui );
}

void Editor::OnRemove(void)
{
    Application::Instance->Disconnect(
        APP_UPDATE,
        this,
        &Editor::onAppUpdate
    );

   m_mainWindow.m_window->Listener( this )
        .Off( WINDOW_RESIZE, &Editor::onMainWindowResize ); 

    m_mainWindow.m_ui->Close( );
    m_mainWindow.m_ui = nullptr;

    m_mainWindow.m_window = nullptr;
}

const EditorWindow &Editor::GetMainWindow(void) const
{
    return m_mainWindow;
}

Project &Editor::GetProject(void)
{
    return m_project;
}

NotificationManager &Editor::GetNotificationManager(void)
{
    return m_notificationManager;
}

Notification Editor::PostNotification(const NotificationConfig &config)
{
    return m_notificationManager.Create( config );
}

void Editor::initializeGraphics(void)
{ 
    graphics::GfxConfig config;

    config.fullscreen = false;
      
    config.handleToWindow =
        static_cast<HWND>( m_mainWindow.m_window->GetPlatformHandle( ) );
       
    config.modelListPath = "Assets/Models/"; 
    config.shaderListPath = URSINE_SHADER_BUILD_DIRECTORY;
    config.textureListPath = "Assets/Textures/";
    config.windowWidth = kDefaultWindowWidth;
    config.windowHeight = kDefaultWindowHeight;
    config.enableDebugInfo = false; 
    config.enableProfiling = false;    
        
    m_graphics->StartGraphics( config );
    m_graphics->Resize( kDefaultWindowWidth, kDefaultWindowHeight ); 
}

void Editor::initializeUI(void)
{
    auto *uiManager = GetCoreSystem( UIManager );
    auto *app = Application::Instance;

    auto argc = app->GetArgC( );
    auto argv = app->GetArgV( );

    std::string entryPoint;

    if (argc == 1)
        entryPoint = kLauncherEntryPoint;
    else
        entryPoint = kEditorEntryPoint;

    m_mainWindow.m_ui = uiManager->CreateView( m_mainWindow.m_window, entryPoint );
}

void Editor::onAppUpdate(EVENT_HANDLER(Application))
{
    EVENT_ATTRS(Application, EventArgs);

    /*auto dt = sender->GetDeltaTime( );

    auto &scene = m_project.GetScene( );

    scene.Update( dt );
    scene.Render( );*/

    m_mainWindow.m_ui->DrawMain( );
}

void Editor::onMainWindowResize(EVENT_HANDLER(Window))
{
    EVENT_ATTRS(Window, WindowResizeArgs);

    m_graphics->Resize( args->width, args->height );

    m_mainWindow.m_ui->SetViewport( {
        0, 0,
        args->width, args->height
    } );
}
