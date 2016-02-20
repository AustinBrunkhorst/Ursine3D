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
    
#include <WindowManager.h>
#include <UIManager.h>   
  
#include <Color.h> 
#include <LightComponent.h> 
  
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
    , m_project( nullptr ) { }

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
    auto *uiManager = GetCoreSystem( UIManager );  

    m_mainWindow.window = windowManager->AddWindow( 
        "Ursine3D Editor", 
        { 0, 0 },
        { static_cast<float>( kDefaultWindowWidth ), static_cast<float>( kDefaultWindowHeight ) },
        SDL_WINDOW_RESIZABLE 
    ); 
     
    m_mainWindow.window->Listener( this )
        .On( WINDOW_RESIZE, &Editor::onMainWindowResize );

    m_mainWindow.window->Listener( this ) 
        .On( WINDOW_FOCUS_CHANGED, &Editor::onFocusChange ); 
     
    m_mainWindow.window->SetLocationCentered( );  
    m_mainWindow.window->Show( true );    
    m_mainWindow.window->SetIcon( "Assets/Resources/Icon.png" );

    m_graphics = GetCoreSystem( graphics::GfxAPI );

    initializeGraphics( );

    m_mainWindow.ui = uiManager->CreateView( m_mainWindow.window, kEditorEntryPoint );

    m_mainWindow.ui->SetViewport( {
        0, 0,
        kDefaultWindowWidth, kDefaultWindowHeight
    } );

    m_project = std::make_shared<Project>( m_mainWindow.ui );

    m_notificationManager.SetUI( m_mainWindow.ui );

    initializeScene( ); 
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
    m_mainWindow.ui = nullptr;

    m_mainWindow.window = nullptr;

    m_project = nullptr;   
}

Window::Handle Editor::GetMainWindow(void) const
{
    return m_mainWindow.window;
}

UIView::Handle Editor::GetMainUI(void) const
{
    return m_mainWindow.ui;
}

std::shared_ptr<Project> Editor::GetProject(void) const
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
        static_cast<HWND>( m_mainWindow.window->GetPlatformHandle( ) );
       
    config.modelListPath = "Assets/Models/"; 
    config.shaderListPath = URSINE_SHADER_BUILD_DIRECTORY;
    config.textureListPath = "Assets/Textures/";
    config.windowWidth = 1366;
    config.windowHeight = 768;      
    config.enableDebugInfo = false; 
    config.enableProfiling = false;    
        
      
    m_graphics->StartGraphics( config );
    m_graphics->Resize( kDefaultWindowWidth, kDefaultWindowHeight ); 
    //m_graphics->SetFullscreenState( true );
     
    //m_mainWindow.window->SetFullScreen( true );
}    

void Editor::initializeScene(void)   
{   
    auto scene = m_project->GetScene( ); 
       
    auto world = scene->GetWorld( ); 
    {
        auto viewport = m_graphics->ViewportMgr.CreateViewport( 0, 0 );
         
        auto &handle = m_graphics->ViewportMgr.GetViewport( viewport );

        handle.SetPosition( 0, 0 ); 
         
        handle.SetBackgroundColor( 255.0f, 0.0f, 0.0f, 1.0f );

        scene->SetViewport( viewport );

        m_graphics->SetGameViewport( viewport ); 
    }

    auto *univLight = world->CreateEntity( "Global Light" );
    {
        auto *component = univLight->AddComponent<ecs::Light>( );

        univLight->GetTransform( )->SetLocalPosition( { 0.0f, 60.0f, 0.0f } );
        univLight->GetTransform( )->SetLocalRotation( { 0.0f, 0.0f, 0.0f } );

        component->SetLightType( ecs::LightType::Directional );
        component->SetRadius( 40.0f );
        component->SetColor( Color( 0.5f, 0.5f, 0.5f, 1.0f ) );
    }

    // this only needs to be called because we manually setup the world
    // rather than loading from a file
    world->DispatchLoad( );
}

void Editor::onAppUpdate(EVENT_HANDLER(Application))
{
    EVENT_ATTRS(Application, EventArgs);

    auto dt = sender->GetDeltaTime( );

    auto scene = m_project->GetScene( );

    scene->Update( dt );
    scene->Render( );

    m_mainWindow.ui->DrawMain( );
}

void Editor::onFocusChange(EVENT_HANDLER( ursine::Window ))
{
    EVENT_ATTRS( Window, WindowFocusArgs );
}

void Editor::onMainWindowResize(EVENT_HANDLER(Window))
{
    EVENT_ATTRS(Window, WindowResizeArgs);

    m_graphics->Resize( args->width, args->height );

    m_mainWindow.ui->SetViewport( {
        0, 0,
        args->width, args->height
    } );
}
