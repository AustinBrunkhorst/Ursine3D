#include "Precompiled.h"

#include "Editor.h"
#include "Project.h"

#include <Application.h>

#include <WindowManager.h>
#include <UIManager.h>
#include <MouseManager.h>

#include <Color.h>

#include <CameraComponent.h>
#include <RenderableComponent.h>

using namespace ursine;

namespace
{
    const auto kEditorEntryPoint = "file:///Assets/UI/Resources/Main.html";
    //const auto kEditorEntryPoint = "www.google.com";

    const auto kEditorClearColor = Color( 0xFF252526 );

    const auto kDefaultWindowWidth = 1280;
    const auto kDefaultWindowHeight = 720;
}

CORE_SYSTEM_DEFINITION( Editor );

Editor::Editor(void)
    : m_graphics( nullptr )
    , m_mainWindow( { nullptr } )
    , m_project( nullptr )
{

}

Editor::~Editor(void)
{

}

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

    m_graphics = GetCoreSystem(graphics::GfxAPI );

    m_project = new Project( );

    initializeGraphics( );

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

void Editor::InitializeScene(void)
{
    m_project->GetScene( ).GetWorld( ).Listener( this )
        .On( ecs::WORLD_ENTITY_ADDED, &Editor::onEntityAdded )
        .On( ecs::WORLD_ENTITY_EDITOR_COMPONENT_CHANGED, &Editor::onComponentChanged );

    auto &scene = m_project->GetScene( );

    auto &world = scene.GetWorld( );


    auto *cameraEntity = world.CreateEntity( "Camera" );
    {
        auto *component = cameraEntity->AddComponent<ecs::Camera>( );

        auto &camera = component->GetCamera( );

        camera.SetPosition( 0.0f, 0.0f );
        camera.SetRenderMode(graphics::VIEWPORT_RENDER_DEFERRED );
        camera.SetDimensions( 1.0f, 1.0f );
        camera.SetPlanes( 0.1f, 700.0f );

        camera.LookAtPoint( { 0.0f, 0.0f, 0.0f } );

        scene.SetEditorCamera( component->GetHandle( ) );
    }

    for (int i = 0; i < 50; ++i)
    {
        auto *entity = world.CreateEntity( );
        {
            auto handle = m_graphics->RenderableMgr.AddRenderable( graphics::RENDERABLE_MODEL3D );

            auto &model = m_graphics->RenderableMgr.GetModel3D( handle );

            if(i == 1)
                model.SetOverdraw(true);

            model.SetEntityUniqueID(entity->GetUniqueID(  ));

            auto name = i & 1 ? "Cube" : "Cylinder";

            entity->SetName( name );

            model.SetModel( name );
            model.SetMaterial( "Cube" );
            model.SetMaterialData(0, 10, 1);

            SMat4 transform;

            entity->GetComponent<ecs::Transform>()->SetWorldPosition(SVec3(i, 0, 0));

            transform.TRS(
                SVec3{ i * 1.0f, 0.0f, 0.0f },
                SQuat{ 0.0f, 0.0f, 0.0f },
                SVec3{ 1.0f, 1.0f, 1.0f }
            );

            model.SetWorldMatrix( transform );

            auto *component = entity->AddComponent<ecs::Renderable>( );

            component->SetHandle( handle );
        }
    }

    //create axis
    auto *xAxis = world.CreateEntity();
    auto *yAxis = world.CreateEntity();
    auto *zAxis = world.CreateEntity();
    {
        auto xHND = m_graphics->RenderableMgr.AddRenderable(graphics::RENDERABLE_MODEL3D);
        auto yHND = m_graphics->RenderableMgr.AddRenderable(graphics::RENDERABLE_MODEL3D);
        auto zHND = m_graphics->RenderableMgr.AddRenderable(graphics::RENDERABLE_MODEL3D);

        auto &xModel = m_graphics->RenderableMgr.GetModel3D(xHND);
        auto &yModel = m_graphics->RenderableMgr.GetModel3D(yHND);
        auto &zModel = m_graphics->RenderableMgr.GetModel3D(zHND);

        xModel.SetModel("Cylinder");
        yModel.SetModel("Cylinder");
        zModel.SetModel("Cylinder");

        xModel.SetMaterial("Blank");
        yModel.SetMaterial("Blank");
        zModel.SetMaterial("Blank");

        xModel.SetEntityUniqueID(xAxis->GetUniqueID());
        yModel.SetEntityUniqueID(yAxis->GetUniqueID());
        zModel.SetEntityUniqueID(zAxis->GetUniqueID());

        xModel.SetColor(Color(1, 0, 0, 1));
        yModel.SetColor(Color(0, 1, 0, 1));
        zModel.SetColor(Color(0, 0, 1, 1));

        xModel.SetMaterialData(3, 0, 0);
        yModel.SetMaterialData(3, 0, 0);
        zModel.SetMaterialData(3, 0, 0);

        xModel.SetOverdraw(true);
        yModel.SetOverdraw(true);
        zModel.SetOverdraw(true);

        SQuat rotZ = SQuat(90, SVec3(1, 0, 0));
        SQuat rotX = SQuat(90, SVec3(0, 0, 1));
        SQuat rotY = SQuat(0, SVec3(0, 0, 1));

        SMat4 xTrans = SMat4(SVec3(0.5, 0, 0));
        SMat4 yTrans = SMat4(SVec3(0, 0.5, 0));
        SMat4 zTrans = SMat4(SVec3(0, 0, 0.5));

        xModel.SetWorldMatrix(xTrans * SMat4(rotX) * SMat4(0.03, 1, 0.03));
        yModel.SetWorldMatrix(yTrans * SMat4(rotY) * SMat4(0.03, 1, 0.03));
        zModel.SetWorldMatrix(zTrans * SMat4(rotZ) * SMat4(0.03, 1, 0.03));

        auto *componentX = xAxis->AddComponent<ecs::Renderable>();
        auto *componentY = yAxis->AddComponent<ecs::Renderable>();
        auto *componentZ = zAxis->AddComponent<ecs::Renderable>();

        componentX->SetHandle(xHND);
        componentY->SetHandle(yHND);
        componentZ->SetHandle(zHND);
    }

    auto *sky = world.CreateEntity( "Skybox" );
    {
        auto skyHND = m_graphics->RenderableMgr.AddRenderable(graphics::RENDERABLE_MODEL3D );

        auto &skybox = m_graphics->RenderableMgr.GetModel3D( skyHND );

        skybox.SetEntityUniqueID(sky->GetUniqueID());

        skybox.SetModel( "Skybox" );
        skybox.SetMaterial( "Skybox" );
        skybox.SetMaterialData( 1, 0, 0 );
        skybox.SetOverdraw(false);
        SQuat rot = SQuat( 90, SVec3( 0, 0, 1 ) );
        SMat4 final = SMat4( rot ) * SMat4( 600, 600, 600 );
        skybox.SetMaterialData(1, 0, 0);
        skybox.SetWorldMatrix( final );

        auto *component = sky->AddComponent<ecs::Renderable>( );

        component->SetHandle( skyHND );
    }

    auto *univLight = world.CreateEntity("Light");
    {
        auto lightHandle = m_graphics->RenderableMgr.AddRenderable(graphics::RENDERABLE_LIGHT);

        auto &light = m_graphics->RenderableMgr.GetLight(lightHandle);

        light.SetEntityUniqueID(univLight->GetUniqueID());
        light.SetType(graphics::Light::LightType::LIGHT_DIRECTIONAL);
        light.SetPosition(0, 0, 0);
        light.SetRadius(40);
        light.SetDirection({ 0.0f, 1.0f, 0.0f });
        light.SetColor(Color::White);

        auto *component = univLight->AddComponent<ecs::Renderable>( );

        component->SetHandle(lightHandle);
    }
}

JSFunction(OnEditorUILoad)
{
    GetCoreSystem( Editor )->InitializeScene( );

    return nullptr;
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
    config.debug = true;

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

void Editor::onAppUpdate(EVENT_HANDLER(Application))
{
    EVENT_ATTRS(Application, EventArgs);

    auto dt = sender->GetDeltaTime( );

    auto &scene = m_project->GetScene( );

    scene.Update( dt );

    //////////////////////////////////////////////////////////////////////////////////////
    //object picking
    static ecs::EntityUniqueID currentID = -1;
    auto id = GetCoreSystem(ursine::graphics::GfxAPI)->GetMousedOverID();

    if(GetCoreSystem(MouseManager)->IsButtonTriggeredDown(MBTN_LEFT) && id != -1)
    {
        if (currentID != -1)
        {
            auto obj = scene.GetWorld().GetEntityUnique(currentID);

            auto renderable = obj->GetComponent<ecs::Renderable>();

            if (renderable)
            {
                auto handle = renderable->GetHandle();
                auto &model = GetCoreSystem(ursine::graphics::GfxAPI)->RenderableMgr.GetModel3D(handle);

                model.SetDebug(false);
            }
        }

        currentID = id;
    }

    //acting upon current selected obj
    if (currentID != -1)
    {
        auto obj = scene.GetWorld().GetEntityUnique(currentID);

        auto renderable = obj->GetComponent<ecs::Renderable>();

        auto transform = obj->GetComponent<ecs::Transform>();

        auto pos = transform->GetWorldPosition();

        GetCoreSystem(ursine::graphics::GfxAPI)->DrawingMgr.SetColor(1, 0, 0, 1);
        GetCoreSystem(ursine::graphics::GfxAPI)->DrawingMgr.DrawLine(pos, pos + SVec3(1, 0, 0));
        GetCoreSystem(ursine::graphics::GfxAPI)->DrawingMgr.SetColor(0, 1, 0, 1);
        GetCoreSystem(ursine::graphics::GfxAPI)->DrawingMgr.DrawLine(pos, pos + SVec3(0, 1, 0));
        GetCoreSystem(ursine::graphics::GfxAPI)->DrawingMgr.SetColor(0, 0, 1, 1);
        GetCoreSystem(ursine::graphics::GfxAPI)->DrawingMgr.DrawLine(pos, pos + SVec3(0, 0, 1));

        if (renderable)
        {
            auto handle = renderable->GetHandle();
            auto &model = GetCoreSystem(ursine::graphics::GfxAPI)->RenderableMgr.GetModel3D(handle);

            model.SetDebug(true);
        }
    }
    //end of object picking
    //////////////////////////////////////////////////////////////////////////////////////

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
        { "field", args->field },
        { "value", args->value.GetType( ).SerializeJson( args->value ) }
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