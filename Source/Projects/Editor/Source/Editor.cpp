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

#include <Timer.h>

#include <ResourcePipelineManager.h>

using namespace ursine;

namespace
{
    const auto kWindowTitle = "Ursine3D Editor";
    const auto kWindowIcon = "Resources/Icon.png";

    const std::string kEntryPointDir = "file:///Assets/UI/Resources/"; 
     
    const auto kEntryPointLauncher = "Launcher.html";
    const auto kEntryPointSplash = "Splash.html";
    const auto kEntryPointEditor = "Editor.html";

    const auto kDefaultPreferencesFile = "Resources/Config/DefaultEditor.prefs";
    const auto kPreferencesFile = "Editor.prefs";
    
    const auto kWindowLocationCentered = Vec2 { -1, -1 };

    const auto kProjectExtension = "ursineproj";

    const auto kResourceFocusTimeout = TimeSpan::FromSeconds( 1.0f );
}

namespace ursine
{
    UIScreenManager *JSGetGlobalScreenManager(void)
    {
        return &GetCoreSystem( Editor )->GetProject( )->GetScene( ).GetScreenManager( );
    }

    CefRefPtr<CefBrowser> JSGetGlobalBrowser(void)
    {
        return GetCoreSystem( Editor )->GetMainWindow( ).GetUI( )->GetBrowser( );
    }
}

CORE_SYSTEM_DEFINITION( Editor );
 
Editor::Editor(void)   
    : m_graphics( nullptr ) 
    , m_project( nullptr ) { }

///////////////////////////////////////////////////////////////////////////////

Editor::~Editor(void) { } 
    
///////////////////////////////////////////////////////////////////////////////
// Misc
///////////////////////////////////////////////////////////////////////////////

const EditorWindow &Editor::GetMainWindow(void) const
{ 
    return m_mainWindow;
}
     
///////////////////////////////////////////////////////////////////////////////

EditorPreferences &Editor::GetPreferences(void)
{
    return m_preferences;
}
     
///////////////////////////////////////////////////////////////////////////////

Project *Editor::GetProject(void)
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

///////////////////////////////////////////////////////////////////////////////

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

    UAssert( ShellExecuteEx( &shExecInfo ) == TRUE,
        "Unable to spawn editor process."
    ); 
     
    CloseHandle( shExecInfo.hProcess );

#endif
     
    Application::Instance->Exit( );
}

///////////////////////////////////////////////////////////////////////////////

void Editor::SetProjectStatus(const std::string &status)
{
    auto title = "Ursine3D Editor - "+ m_project->GetConfig( ).title + " - ";

    title += status;

    m_mainWindow.m_window->SetTitle( title );
}

///////////////////////////////////////////////////////////////////////////////
// Core System
///////////////////////////////////////////////////////////////////////////////

void Editor::OnInitialize(void)
{
    m_project = new Project( );

    m_project->GetScene( ).Listener( this )
        .On( SCENE_WORLD_CHANGED, &Editor::onSceneWorldChanged );

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
}

///////////////////////////////////////////////////////////////////////////////

void Editor::OnRemove(void)
{
    // only write the preferences if we were in the editor
    if (m_startupConfig.updateHandler == &Editor::onEditorUpdate)
        writePreferences( );

    Application::Instance->Disconnect(
        APP_UPDATE,
        this,
        m_startupConfig.updateHandler
    );

    m_mainWindow.m_window->Listener( this )
        .Off( WINDOW_RESIZE, &Editor::onMainWindowResize )
        .Off( WINDOW_FOCUS_CHANGED, &Editor::onMainWindowFocusChanged );

    m_buildPipelineFocusTimeout.Cancel( );

    m_mainWindow.m_ui->Listener( this )
        .Off( UI_LOADED, &Editor::onUILoaded )
        .Off( UI_POPUP_CREATED, &Editor::onUIPopup );

    m_mainWindow.m_ui->Close( );
    m_mainWindow.m_ui = nullptr;

    m_mainWindow.m_window = nullptr;

    m_project->GetScene( ).Listener( this )
        .Off( SCENE_WORLD_CHANGED, &Editor::onSceneWorldChanged );

    delete m_project;

    m_project = nullptr;   
}

///////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////

void Editor::writePreferences(void)
{
    auto window = m_mainWindow.m_window;

    m_preferences.windowSize = window->GetSize( );
    m_preferences.windowLocation = window->GetLocation( );

    m_preferences.lastOpenWorld = to_string( m_project->GetLastOpenedWorld( ) );

    auto prefsJson = meta::Type::SerializeJson<EditorPreferences>( m_preferences ).dump( true );

    fs::WriteAllText( kPreferencesFile, prefsJson );
}

///////////////////////////////////////////////////////////////////////////////

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

    auto window = m_mainWindow.m_window = windowManager->AddWindow(
        kWindowTitle,
        Vec2::Zero( ),
        m_startupConfig.windowSize,
        m_startupConfig.windowFlags
    );

    auto &size = window->GetSize( );

    window->SetSize( size * window->GetDPIScaleFactor( ) );

    window->Listener( this )
        .On( WINDOW_RESIZE, &Editor::onMainWindowResize );
    
    window->SetLocationCentered( );
    window->SetIcon( kWindowIcon );
}  

///////////////////////////////////////////////////////////////////////////////

void Editor::initializeGraphics(void)
{ 
    m_graphics = GetCoreSystem( graphics::GfxAPI );

    graphics::GfxConfig gfxConfig;

    gfxConfig.fullscreen = false;
      
    auto window = m_mainWindow.m_window;
    auto &size = window->GetSize( );

    gfxConfig.handleToWindow =
        static_cast<HWND>(window->GetPlatformHandle( ) );
       
    gfxConfig.shaderListPath = URSINE_SHADER_BUILD_DIRECTORY;
    gfxConfig.windowWidth = static_cast<unsigned>( size.X( ) );
    gfxConfig.windowHeight = static_cast<unsigned>( size.Y( ) );
    gfxConfig.enableDebugInfo = false;
    gfxConfig.enableProfiling = false;

    m_graphics->StartGraphics( gfxConfig );
    m_graphics->Resize( gfxConfig.windowWidth, gfxConfig.windowHeight );
      
    auto viewport = m_graphics->ViewportMgr.CreateViewport( 0, 0 );
       
    auto &handle = m_graphics->ViewportMgr.GetViewport( viewport );
        
    handle.SetPosition( 0, 0 );
      
    m_project->GetScene( ).SetViewport( viewport );
     
    m_graphics->SetGameViewport( viewport );
}    

///////////////////////////////////////////////////////////////////////////////

void Editor::initializeUI(void)
{   
    auto *uiManager = GetCoreSystem( UIManager );

    auto window = m_mainWindow.m_window;
    auto &size = window->GetSize( );

    m_mainWindow.m_ui = uiManager->CreateView( 
        window, 
        m_startupConfig.uiEntryPoint 
    );

    m_mainWindow.m_ui->Listener( this )
        .On( UI_LOADED, &Editor::onUILoaded )
        .On( UI_POPUP_CREATED, &Editor::onUIPopup );

    m_mainWindow.m_ui->SetViewport( {
        0, 0,
        static_cast<int>( size.X( ) ), 
        static_cast<int>( size.Y( ) )
    } );

    m_notificationManager.SetUI( m_mainWindow.m_ui );
}

///////////////////////////////////////////////////////////////////////////////
       
void Editor::initializeProject(const std::string &fileName)
{
    fs::path projectFileName;

    try
    {
        projectFileName = fs::canonical( fileName );
    }
    catch (...)
    {
        UError( "Unable to resolve project file name." ); 
    } 

    std::string projectJsonText;

    UAssert( fs::LoadAllText( projectFileName.string( ), projectJsonText ),
        "Unable to open project file." 
    );

    std::string projectJsonError;

    auto projectJson = Json::parse( projectJsonText, projectJsonError );

    UAssert( projectJsonError.empty( ),
        "Unable to parse project settings."
    );

    auto project = meta::Type::DeserializeJson<ProjectConfig>( projectJson );

    project.projectFile = projectFileName;

    m_project->initialize( project );
         
    auto &recentProjects = m_preferences.recentProjects;

    // remove it if it existed, then add it to the beginning
    recentProjects.Remove( projectFileName.string( ) );
    recentProjects.Insert( recentProjects.begin( ), projectFileName.string( ) );

    auto &resourcePipeline = m_project->GetResourcePipeline( );

    resourcePipeline.Listener( this )
        .On( rp::RP_BUILD_RESOURCE_START, &Editor::onPipelinePreBuildItemStart )
        .On( rp::RP_BUILD_RESOURCE_PREVIEW_START, &Editor::onPipelinePreBuildItemPreviewStart )
        .On( rp::RP_BUILD_COMPLETE, &Editor::onPipelinePreBuildComplete );

    resourcePipeline.Build( );
}

///////////////////////////////////////////////////////////////////////////////

void Editor::exitSplashScreen(void)
{
    auto window = m_mainWindow.m_window;

    // will be shown in onUILoaded( )
    window->Show( false );
    window->SetResizable( true );
    window->SetBordered( true );
    window->SetSize( m_preferences.windowSize );

    window->SetTitle( "Ursine3D Editor - "+ m_project->GetConfig( ).title );

    if (m_preferences.windowLocation == kWindowLocationCentered)
        window->SetLocationCentered( );
    else
        window->SetLocation( m_preferences.windowLocation ); 

    m_mainWindow.m_ui->GetBrowser( )->GetMainFrame( )->LoadURL( 
        kEntryPointDir + kEntryPointEditor 
    );

    ursine::GUID lastWorldGUID;

    if (m_preferences.lastOpenWorld.empty( ))
        lastWorldGUID = kNullGUID;
    else
        lastWorldGUID = GUIDStringGenerator( )( m_preferences.lastOpenWorld );

    auto &resourceManager = m_project->GetScene( ).GetResourceManager( );

    m_project->GetScene( ).GetScreenManager( ).SetUI( m_mainWindow.m_ui );

    m_project->initializeScene(
        resourceManager.CreateReference( lastWorldGUID )
    );

    m_mainWindow.m_window->Listener( this )
        .On( WINDOW_FOCUS_CHANGED, &Editor::onMainWindowFocusChanged );
}

///////////////////////////////////////////////////////////////////////////////
// Event Handlers
///////////////////////////////////////////////////////////////////////////////

void Editor::onUILoaded(EVENT_HANDLER(ursine::UIView))
{
    // ensure there is no flashing
    Timer::Create( 500 ).Completed( [=]
    {
        m_mainWindow.m_window->Show( true );
    } );
}

///////////////////////////////////////////////////////////////////////////////

void Editor::onUIPopup(EVENT_HANDLER(ursine::UIView))
{
    EVENT_ATTRS(ursine::UIView, ursine::UIPopupArgs);
}

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

    auto &scene = m_project->GetScene( );

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

///////////////////////////////////////////////////////////////////////////////

void Editor::onMainWindowFocusChanged(EVENT_HANDLER(Window))
{
    EVENT_ATTRS(Window, WindowFocusArgs);

    m_buildPipelineFocusTimeout.Cancel( );

    if (args->focused)
    {
        m_buildPipelineFocusTimeout = Timer::Create( kResourceFocusTimeout ).Completed( [=] {
            m_project->GetResourcePipeline( ).AllowFileActionProcessing( true );
        } );
    } 
    else
    {
        m_project->GetResourcePipeline( ).AllowFileActionProcessing( false );
    }
}

///////////////////////////////////////////////////////////////////////////////

void Editor::onPipelinePreBuildItemStart(EVENT_HANDLER(rp::ResourcePipelineManager))
{
    EVENT_ATTRS(rp::ResourcePipelineManager, rp::ResourceBuildArgs);

    auto fileName = fs::MakeRelativePath(
        sender->GetConfig( ).resourceDirectory,
        args->resource->GetSourceFileName( )
    );

    m_mainWindow.m_ui->Message( UI_CMD_BROADCAST, "ResourcePipeline", "preBuildItemStart", 
        Json::object {
            { "item", fileName.string( ) },
            { "progress", args->progress }
        } 
    ); 
}

///////////////////////////////////////////////////////////////////////////////

void Editor::onPipelinePreBuildItemPreviewStart(EVENT_HANDLER(rp::ResourcePipelineManager))
{
    EVENT_ATTRS(rp::ResourcePipelineManager, rp::ResourceBuildArgs);

    auto fileName = fs::MakeRelativePath(
        sender->GetConfig( ).resourceDirectory,
        args->resource->GetSourceFileName( )
    );

    m_mainWindow.m_ui->Message( UI_CMD_BROADCAST, "ResourcePipeline", "preBuildItemPreviewStart", 
        Json::object {
            { "item", fileName.string( ) },
            { "progress", args->progress }
        } 
    );
}

///////////////////////////////////////////////////////////////////////////////

void Editor::onPipelinePreBuildComplete(EVENT_HANDLER(rp::ResourcePipelineManager))
{
    EVENT_ATTRS(rp::ResourcePipelineManager, EventArgs);

    sender->Listener( this )
        .Off( rp::RP_BUILD_RESOURCE_START, &Editor::onPipelinePreBuildItemStart )
        .Off( rp::RP_BUILD_RESOURCE_PREVIEW_START, &Editor::onPipelinePreBuildItemPreviewStart )
        .Off( rp::RP_BUILD_COMPLETE, &Editor::onPipelinePreBuildComplete );

    m_mainWindow.m_ui->Message( UI_CMD_BROADCAST, "ResourcePipeline", "preBuildComplete", { } );

    Application::PostMainThread( std::bind( &Editor::exitSplashScreen, this ) );
}

///////////////////////////////////////////////////////////////////////////////

void Editor::onSceneWorldChanged(EVENT_HANDLER(Scene))
{
    EVENT_ATTRS(Scene, SceneWorldChangedArgs);

    if (args->reference)
    {
        auto item = m_project->GetResourcePipeline( ).GetItem( args->reference->GetGUID( ) );

        if (item)
            SetProjectStatus( item->GetDisplayName( ) );
        else
            SetProjectStatus( "Unknown World" );
    }
    else
    {
        SetProjectStatus( "Untitled World" );
    }
}