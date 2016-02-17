﻿/* ----------------------------------------------------------------------------
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
#include <Timer.h>

using namespace ursine;

namespace
{
    const auto kWindowTitle = "Ursine3D Editor";

    const std::string kEntryPointDir = "file:///Assets/UI/Resources/";

    const auto kEntryPointLauncher = "Launcher.html";
    const auto kEntryPointSplash = "Splash.html";
    const auto kEntryPointEditor = "Editor.html";

    const auto kDefaultPreferencesFile = "Assets/Config/DefaultEditor.prefs";
    const auto kPreferencesFile = "Editor.prefs";
    
    const auto kWindowLocationCentered = Vec2 { -1, -1 };

    const auto kProjectExtension = "ursineproj";
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

const EditorPreferences &Editor::GetPreferences(void) const
{
    return m_preferences;
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
// Projects
///////////////////////////////////////////////////////////////////////////////

void Editor::CreateNewProject(const std::string &name, const std::string &directory)
{
    auto sanitizedName = fs::SafeFileName( name, '_' );

    auto projectDirectory = fs::path( directory ) / sanitizedName;

    auto projectFileName = (projectDirectory / sanitizedName)
        .replace_extension( kProjectExtension );

    // make sure the directory exists
    fs::create_directory( projectDirectory );

    auto projectConfig = ProjectConfig::CreateDefault( name );

    auto projectData = 
        meta::Type::SerializeJson<ProjectConfig>( projectConfig );

    fs::WriteAllText( projectFileName.string( ), projectData.dump( true ) );

    LoadProject( projectFileName.string( ) );
}

void Editor::LoadProject(const std::string &filename)
{
#if defined(PLATFORM_WINDOWS)

    SHELLEXECUTEINFO shExecInfo;

    shExecInfo.cbSize = sizeof( SHELLEXECUTEINFO );

    std::string args;

    args += '"';
    args += filename;
    args += '"';

    shExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    shExecInfo.hwnd = nullptr;
    shExecInfo.lpVerb = nullptr;
    shExecInfo.lpFile = "Editor.exe";
    shExecInfo.lpParameters = args.c_str( );
    shExecInfo.lpDirectory = nullptr;
    shExecInfo.nShow = SW_NORMAL;
    shExecInfo.hInstApp = nullptr;

    ShellExecuteEx( &shExecInfo );

#endif

    Application::Instance->Exit( );
}

///////////////////////////////////////////////////////////////////////////////
// Core System
///////////////////////////////////////////////////////////////////////////////

void Editor::OnInitialize(void)
{
    loadPreferences( );

    auto projectFileName = findAvailableProject( );

    auto hasProject = !projectFileName.empty( );

    // configure the splash screen
    if (hasProject)
    {
        m_startupConfig.uiEntryPoint = kEntryPointDir + kEntryPointSplash;
        m_startupConfig.windowFlags = SDL_WINDOW_RESIZABLE;
        m_startupConfig.windowSize = { 475, 275 };
        m_startupConfig.updateHandler = &Editor::onEditorUpdate;
    }
    // couldn't find a project, configure the launcher
    else
    {
        m_startupConfig.uiEntryPoint = kEntryPointDir + kEntryPointLauncher;
        m_startupConfig.windowSize = { 660, 430 };
        m_startupConfig.windowFlags = 0;
        m_startupConfig.updateHandler = &Editor::onLauncherUpdate;
    }

    startup( );

    if (hasProject)
    {
        // this is a fix to a problem with SDL, only for the splash screen
        m_mainWindow.m_window->SetBordered( false );
        m_mainWindow.m_window->SetResizable( false );

        initializeProject( projectFileName );
    }

    // eliminate flashy stuff
    Timer::Create( 750 ).Completed( [=] {
        m_mainWindow.m_window->Show( true );
    } );
}

///////////////////////////////////////////////////////////////////////////////

void Editor::OnRemove(void)
{
    writePreferences( );

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

void Editor::loadPreferences(void)
{
    std::string prefsJsonText;

    if (!fs::LoadAllText( kPreferencesFile, prefsJsonText ))
    {
        UAssert( fs::LoadAllText( kDefaultPreferencesFile, prefsJsonText ),
            "Unable to load default preferences."
        );
    }

    std::string prefsJsonError;

    auto prefsJson = Json::parse( prefsJsonText, prefsJsonError );

    UAssert( prefsJsonError.empty( ),
        "Unable to parse preferences."
    );

    m_preferences = meta::Type::DeserializeJson<EditorPreferences>( prefsJson );
}

void Editor::writePreferences(void)
{
    auto window = m_mainWindow.m_window;

    m_preferences.windowSize = window->GetSize( );
    m_preferences.windowLocation = window->GetLocation( );

    auto prefsJson = meta::Type::SerializeJson<EditorPreferences>( m_preferences ).dump( true );

    fs::WriteAllText( kPreferencesFile, prefsJson );
}

std::string Editor::findAvailableProject(void) const
{
    auto *app = Application::Instance;

    auto argc = app->GetArgC( );
    auto argv = app->GetArgV( );

    // a project was passed in - make sure it exists
    if (argc > 1 && fs::exists( argv[ 1 ] ))
        return argv[ 1 ];

    // take first recent project that exists
    for (auto &file : m_preferences.recentProjects)
    {
        if (fs::exists( file ))
            return file;
    }

    // couldn't find one
    return "";
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
        m_startupConfig.windowFlags
    );

    m_mainWindow.m_window->Listener( this )
        .On( WINDOW_RESIZE, &Editor::onMainWindowResize );
    
    m_mainWindow.m_window->SetLocationCentered( );
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

void Editor::initializeProject(const std::string &filename)
{
    std::string projectJsonText;

    UAssert( fs::LoadAllText( filename, projectJsonText ),
        "Unable to open project file."
    );

    std::string projectJsonError;

    auto projectJson = Json::parse( projectJsonText, projectJsonError );

    UAssert( projectJsonError.empty( ),
        "Unable to parse project settings."
    );

    auto project = meta::Type::DeserializeJson<ProjectConfig>( projectJson );

    project.rootDirectory = fs::path( filename ).parent_path( );

    m_project.initialize( project );

    auto &recentProjects = m_preferences.recentProjects;

    // add it as a recent project if it doesn't already exist
    if (recentProjects.Find( filename ) == recentProjects.end( ))
        recentProjects.Push( filename );
}

void Editor::exitSplashScreen(void)
{
    auto window = m_mainWindow.m_window;

    window->Show( false );
    window->SetResizable( true );
    window->SetBordered( true );
    window->SetSize( m_preferences.windowSize );

    if (m_preferences.windowLocation == kWindowLocationCentered)
        window->SetLocationCentered( );
    else
        window->SetLocation( m_preferences.windowLocation );

    m_mainWindow.m_ui->GetBrowser( )->GetMainFrame( )->LoadURL( 
        kEntryPointDir + kEntryPointEditor 
    );

    // give the window time to adjust
    Timer::Create( 700 ).Completed( [=] {
        window->Show( true );
    } );
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
