#include "Precompiled.h"

#include "Editor.h"
#include "Project.h"

#include <Application.h>

#include <WindowManager.h>
#include <UIManager.h>

#include <Color.h>

#include <CameraComponent.h>
#include <RenderableComponent.h>
#include <LightComponent.h>
#include <Model3DComponent.h>

using namespace ursine;

namespace
{
    const auto kEditorEntryPoint = "file:///Assets/UI/Resources/Main.html";

    const auto kEditorClearColor = Color( 0xFF252526 );

    const auto kDefaultWindowWidth = 1280;
    const auto kDefaultWindowHeight = 720;
}

CORE_SYSTEM_DEFINITION( Editor ) ;

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
    m_mainWindow.window->SetIcon( "Assets/Resources/Icon.png" );

    m_graphics = GetCoreSystem( graphics::GfxAPI );

    m_project = new Project( );

    initializeGraphics( );
    initializeScene( );

    m_mainWindow.ui = uiManager->CreateView( m_mainWindow.window, kEditorEntryPoint );

    m_mainWindow.ui->SetViewport( {
        0, 0,
        kDefaultWindowWidth, kDefaultWindowHeight
    } );
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

    delete m_mainWindow.window;

    m_mainWindow.window = nullptr;

    delete m_project;

    m_project = nullptr;
}

Project *Editor::GetProject(void) const
{
    return m_project;
}

void Editor::initializeGraphics(void)
{
    graphics::GfxConfig config;

    config.Fullscreen_ = false;

    config.HandleToWindow_ =
            static_cast<HWND>( m_mainWindow.window->GetPlatformHandle( ) );

    config.ModelListPath_ = "Assets/Models/";
    config.ShaderListPath_ = URSINE_SHADER_BUILD_DIRECTORY;
    config.TextureListPath_ = "Assets/Textures/";
    config.WindowWidth_ = kDefaultWindowWidth;
    config.WindowHeight_ = kDefaultWindowHeight;

    URSINE_TODO( "..." );

    config.m_renderUI = true;
    config.debug = false;

    config.Profile_ = false;

    m_graphics->StartGraphics( config );
    m_graphics->Resize( kDefaultWindowWidth, kDefaultWindowHeight );

    auto &scene = m_project->GetScene( );
    {
        auto viewport = m_graphics->ViewportMgr.CreateViewport(
            static_cast<int>( 0.85f * kDefaultWindowWidth ),
            static_cast<int>( kDefaultWindowHeight - (30.0f + 27.0f) )
        );

        auto &handle = m_graphics->ViewportMgr.GetViewport( viewport );

        handle.SetPosition(
            static_cast<int>( 0.15f * kDefaultWindowWidth ),
            static_cast<int>( 30.0f + 27.0f )
        );

        handle.SetBackgroundColor( 255.0f, 0.0f, 0.0f, 1.0f );

        scene.SetViewport( viewport );

        m_graphics->SetGameViewport( viewport );
    }
}

void Editor::initializeScene(void)
{
    auto &scene = m_project->GetScene( );

    auto &world = scene.GetWorld( );

    auto *cameraEntity = world.CreateEntity( "Camera" );
    {
        auto *component = cameraEntity->AddComponent<ecs::Camera>( );

        auto &camera = component->GetCamera( );

        camera.SetPosition( 0.0f, 0.0f );
        camera.SetRenderMode( graphics::VIEWPORT_RENDER_DEFERRED );
        camera.SetDimensions( 1.0f, 1.0f );
        camera.SetPlanes( 0.1f, 700.0f );

        camera.LookAtPoint( { 0.0f, 0.0f, 0.0f } );

        scene.SetEditorCamera( component->GetHandle( ) );
    }

    for (int i = 0; i < 25; ++i)
    {
        auto *entity_char = world.CreateEntity( );
        auto *entity_cube = world.CreateEntity( );
        {
            entity_char->AddComponent<ecs::Renderable>();
            auto model = entity_char->AddComponent<ecs::Model3D>();

            auto name = "Character";

            entity_char->SetName( name );

            model->SetModel( name );

            auto transform = entity_char->GetTransform( );

            transform->SetWorldPosition( SVec3{ i * 1.0f, 0.0f, 0.0f } );
            transform->SetWorldRotation( SQuat{ 0.0f, 0.0f, 0.0f } );
            transform->SetWorldScale( SVec3{ 1.0f, 1.0f, 1.0f } );
        }
        {
            entity_cube->AddComponent<ecs::Renderable>();
            auto model = entity_cube->AddComponent<ecs::Model3D>();

            auto name = "Cube";

            entity_cube->SetName(name);

            model->SetModel(name);

            auto transform = entity_cube->GetTransform();

            transform->SetWorldPosition(SVec3{ i * 1.0f, 0.0f, 0.0f });
            transform->SetWorldRotation(SQuat{ 0.0f, 0.0f, 0.0f });
            transform->SetWorldScale(SVec3{ 1.0f, 1.0f, 1.0f });
        }

        // parent the character to the cube
        entity_cube->GetTransform( )->AddChild( entity_char->GetTransform( ) );
    }

    auto *sky = world.CreateEntity( "Skybox" );
    {
        auto skyHND = m_graphics->RenderableMgr.AddRenderable( graphics::RENDERABLE_MODEL3D );

        auto &skybox = m_graphics->RenderableMgr.GetModel3D( skyHND );

        skybox.SetModel( "Skybox" );
        skybox.SetMaterial( "Skybox" );
        skybox.SetMaterialData( 1, 0, 0 );

        SQuat rot = SQuat( 90, SVec3( 0, 0, 1 ) );
        SMat4 final = SMat4( rot ) * SMat4( 600, 600, 600 );
        skybox.SetMaterialData( 1, 0, 0 );
        skybox.SetWorldMatrix( final );

        auto *component = sky->AddComponent<ecs::Renderable>( );

        component->SetHandle( skyHND );
    }

    auto *univLight = world.CreateEntity( "Global Light" );
    {
        auto *component = univLight->AddComponent<ecs::Light>( );

        component->SetType( ecs::LightType::Point );
        component->SetPosition( { 0.0f, 0.0f, 0.0f } );
        component->SetRadius( 40.0f );
        component->SetDirection( { 0.0f, 1.0f, 0.0f } );
        component->SetColor( Color::White );
    }

    m_project->GetScene( ).GetWorld( ).Listener( this )
        .On( ecs::WORLD_ENTITY_ADDED, &Editor::onEntityAdded )
        .On( ecs::WORLD_ENTITY_EDITOR_COMPONENT_CHANGED, &Editor::onComponentChanged );
}

void Editor::onAppUpdate(EVENT_HANDLER(Application))
{
    EVENT_ATTRS(Application, EventArgs);

    auto dt = sender->GetDeltaTime( );

    auto &scene = m_project->GetScene( );

    scene.Update( dt );

    m_graphics->StartFrame( );

    scene.Render( );

    m_mainWindow.ui->DrawMain( );

    m_graphics->EndFrame( );
}

void Editor::onEntityAdded(EVENT_HANDLER(ecs::World))
{
    EVENT_ATTRS(ecs::World, ecs::EntityEventArgs);

    Json message = Json::object {
        { "uniqueID", static_cast<int>( args->entity->GetUniqueID( ) ) }
    };

    m_mainWindow.ui->Message( UI_CMD_BROADCAST, "EntityManager", "EntityAdded", message );
}

void Editor::onComponentChanged(EVENT_HANDLER(ecs::World))
{
    EVENT_ATTRS(ecs::World, ecs::EditorComponentChangedArgs);

    Json message = Json::object {
        { "uniqueID", static_cast<int>( args->entity->GetUniqueID( ) ) },
        { "component", args->component->GetType( ).GetName( ) },
        { "field", args->field },
        { "value", args->value.SerializeJson( ) }
    };

    m_mainWindow.ui->Message( UI_CMD_BROADCAST, "EntityManager", "ComponentChanged", message );
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