#include "Precompiled.h"

#include <CFBXLoader.h>

#include "Editor.h"
#include "Project.h"

#include <Application.h>

#include <WindowManager.h>
#include <UIManager.h>

#include <Color.h> 

#include <SystemManager.h>
#include <CameraComponent.h> 
#include <RenderableComponent.h>   
#include <LightComponent.h>
#include <Model3DComponent.h>
#include <CapsuleColliderComponent.h>
#include <BoxColliderComponent.h>
#include <AnimationBuilder.h>

#include "CharacterControllerComponent.h" 
#include "EditorCameraSystem.h"

#include "PhysicsSystem.h"

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

    m_mainWindow.window->SetLocationCentered( );
    m_mainWindow.window->Show( true );
    m_mainWindow.window->SetFullScreen( false );
    m_mainWindow.window->SetIcon( "Assets/Resources/Icon.png" );

    m_graphics = GetCoreSystem( graphics::GfxAPI );

    initializeGraphics( );

    m_mainWindow.ui = uiManager->CreateView( m_mainWindow.window, kEditorEntryPoint );

    m_mainWindow.ui->SetViewport( {
        0, 0,
        kDefaultWindowWidth, kDefaultWindowHeight
    } );

    m_project = std::make_shared<Project>( m_mainWindow.ui );

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

void Editor::initializeGraphics(void)
{
    graphics::GfxConfig config;

    config.Fullscreen_ = m_mainWindow.window->IsFullScreen( );

    config.HandleToWindow_ =
        static_cast<HWND>( m_mainWindow.window->GetPlatformHandle( ) );

    config.ModelListPath_ = "Assets/Models/";
    config.ShaderListPath_ = URSINE_SHADER_BUILD_DIRECTORY;
    config.TextureListPath_ = "Assets/Textures/";
    config.WindowWidth_ = static_cast<unsigned>(m_mainWindow.window->GetSize( ).X( ));
    config.WindowHeight_ = static_cast<unsigned>(m_mainWindow.window->GetSize( ).Y( ));

    URSINE_TODO( "..." );

    config.m_renderUI = true;
    config.debug = false;

    config.Profile_ = false;

    m_graphics->StartGraphics( config );
    m_graphics->Resize( kDefaultWindowWidth, kDefaultWindowHeight );
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

    world->DispatchLoad( );

    auto *cameraEntity = world->CreateEntity( "Camera" );
    {
        auto *component = cameraEntity->AddComponent<ecs::Camera>( );
		 
        auto camera = component->GetCamera( );

        camera->SetPosition( 0.0f, 0.0f );
		camera->SetRenderMode(graphics::VIEWPORT_RENDER_FORWARD );
        camera->SetDimensions( 1.0f, 1.0f ); 
        camera->SetPlanes( 0.1f, 700.0f );
              
        camera->LookAtPoint( { 0.0f, 0.0f, 0.0f } );
		 
    }
}

void Editor::onAppUpdate(EVENT_HANDLER(Application))
{
    EVENT_ATTRS(Application, EventArgs);
     
    auto dt = sender->GetDeltaTime( );



    auto scene = m_project->GetScene( );
	  
    scene->Update( dt ); 


    m_graphics->StartFrame( );  

    scene->Render( ); 

    m_mainWindow.ui->DrawMain( );  

    m_graphics->EndFrame( );

    static bool hi = false;

    if (!hi)
    {
        const int numRays = 5;
        const float incAngle = math::PI_2 / numRays;
        const float radius = 4.0f;

        for (int i = 0; i < numRays; ++i)
        {
            float ang = i * incAngle;

            SVec3 start(0.0f, 40.0f, 0.0f);
            SVec3 end(cos(ang) * radius, -1.0f, sin(ang) * radius);

            physics::RaycastInput input(start, end);
            physics::RaycastOutput output;

            m_project->GetScene( )->GetWorld( )->GetEntitySystem(ursine::ecs::PhysicsSystem)
                ->Raycast(input, output, physics::RAYCAST_CLOSEST_HIT, true, 0.016f);
        }
    }
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
