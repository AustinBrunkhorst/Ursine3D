/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Project.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "Project.h"

#include "ResourcePipelineConfig.h"
#include "EditorUIResourceHandlerFactory.h"

#include <WorldSerializer.h>
#include <WorldData.h>

#include <LightComponent.h>
#include <WorldConfigComponent.h>
#include <Color.h>

#include <UIConfig.h>

using namespace ursine;

namespace
{
    const auto kBuiltInResourcesDirectory = "Resources/EditorTools";

    const auto kPreferencesDirectory = "Preferences";
    const auto kResourcesTempDirectory = "Temp";
    const auto kResourcesBuildDirectory = "Resources";
}

Project::Project(void)
    : m_preferences( this )
    , m_gameBuilder( this )
    , m_gameInstaller( this )
    , m_gameContext( nullptr )
    , m_sceneManager( nullptr )
    , m_entityManager( nullptr )
    , m_pipelineManager( nullptr )
    , m_lastOpenedWorld( kNullGUID )
{
    m_scene.Listener( this )
        .On( SCENE_WORLD_CHANGED, &Project::onSceneWorldChanged )
        .On( EDITOR_SCENE_PLAYSTATE_CHANGED, &Project::onScenePlayStateChanged );

    m_resourcePipeline.Listener( this )
        .On( rp::RP_RESOURCE_MODIFIED, &Project::onResourceModified );

    m_builtInResourceManager.SetResourceDirectory( kBuiltInResourcesDirectory );
}

Project::~Project(void)
{
    m_scene.Listener( this )
        .Off( SCENE_WORLD_CHANGED, &Project::onSceneWorldChanged )
        .Off( EDITOR_SCENE_PLAYSTATE_CHANGED, &Project::onScenePlayStateChanged );

    m_resourcePipeline.Listener( this )
        .Off( rp::RP_RESOURCE_MODIFIED, &Project::onResourceModified );

    delete m_gameContext;
    delete m_sceneManager;
    delete m_entityManager;
    delete m_pipelineManager;
}

ProjectConfig &Project::GetConfig(void)
{
    return m_config;
}

void Project::WriteConfig(void)
{
    auto data = meta::Type::SerializeJson<ProjectConfig>( m_config );

    UAssert( fs::WriteAllText( m_config.projectFile.string( ), data.dump( true ) ),
        "Unable to write project configuration.\nfile: %s",
        m_config.projectFile.string( ).c_str( )
    );
}

ProjectPreferenceStore &Project::GetPreferenceStore(void)
{
    return m_preferences;
}

ProjectGameBuilder &Project::GetGameBuilder(void)
{
    return m_gameBuilder;
}

ProjectGameInstaller &Project::GetGameInstaller(void)
{
    return m_gameInstaller;
}

rp::ResourcePipelineManager &Project::GetResourcePipeline(void)
{
    return m_resourcePipeline;
}

resources::ResourceManager &Project::GetBuiltInResourceManager(void)
{
    return m_builtInResourceManager;
}

EditorScene &Project::GetScene(void)
{
    return m_scene;
}

void Project::SetEmptyScene(void)
{
    auto world = std::make_shared<ecs::World>( );

    auto univLight = world->CreateEntity( "Global Light" );
    {
        auto *component = univLight->AddComponent<ecs::Light>( );

        univLight->GetTransform( )->SetLocalPosition( { 0.0f, 60.0f, 0.0f } );

        component->SetLightType( ecs::LightType::Directional );
        component->SetRadius( 40.0f );
        component->SetColor( Color( 0.5f, 0.5f, 0.5f, 1.0f ) );
    }

    m_scene.SetActiveWorld( world );

    world->GetSettings( )->GetComponent<ecs::WorldConfig>( )->SetInEditorMode( true );
}

const ursine::GUID &Project::GetLastOpenedWorld(void)
{
    return m_lastOpenedWorld;
}

bool Project::CreateEditorResource(const ursine::GUID &resourceGUID) const
{
    try
    {
        auto resource = m_resourcePipeline.GetItem( resourceGUID );

        if (!resource)
            return false;

        auto buildFile = resource->GetBuildFileName( );
        auto targetFile = kBuiltInResourcesDirectory / buildFile.filename( );

        boost::system::error_code e;

        copy_file( buildFile, targetFile, fs::copy_option::overwrite_if_exists, e );

        return true;
    }
    catch (...)
    {
        return false;
    }
}

void Project::initialize(const ProjectConfig &config)
{
    m_config = config;

    auto rootDirectory = m_config.projectFile.parent_path( );

    m_config.preferencesDirectory = rootDirectory / config.buildDirectory / kPreferencesDirectory;

    if (!exists( m_config.preferencesDirectory ))
        create_directories( m_config.preferencesDirectory );

    rp::ResourcePipelineConfig resourceConfig;

    resourceConfig.resourceDirectory = rootDirectory / config.resourceDirectory;

    resourceConfig.tempDirectory = rootDirectory / config.buildDirectory;
    {
        // add the temp directory
        resourceConfig.tempDirectory /= kResourcesTempDirectory;
    }

    resourceConfig.buildDirectory = rootDirectory / config.buildDirectory;
    {
        // add the resource directory
        resourceConfig.buildDirectory /= kResourcesBuildDirectory;
    }

    m_scene.GetResourceManager( ).SetResourceDirectory(
        resourceConfig.buildDirectory
    );

    m_resourcePipeline.SetConfig( resourceConfig );
}

void Project::initializeScene(const resources::ResourceReference &startingWorld)
{
    m_gameContext = new EditorGameContext( this );

    m_scene.SetGameContext( m_gameContext );

    m_sceneManager = new EditorSceneManager( this );
    m_entityManager = new EditorEntityManager( this );

    auto world = startingWorld.Load<resources::WorldData>( m_scene.GetResourceManager( ) );

    if (world)
    {
        m_scene.SetActiveWorld( startingWorld );
    }
    else
    {
        SetEmptyScene( );
    }

    m_pipelineManager = new EditorResourcePipelineManager( this );

    m_resourcePipeline.WatchResourceDirectory( );

    // create the UI resource factory
    CefRegisterSchemeHandlerFactory( 
        "http", 
        kUIGameResourceDomain, 
        new EditorUIResourceHandlerFactory( ) 
    );
}

void Project::onSceneWorldChanged(EVENT_HANDLER(Scene))
{
    EVENT_ATTRS(Scene, SceneWorldChangedArgs);

    if (args->reference)
    {
        m_lastOpenedWorld = args->reference->GetGUID( );
    }
    else
    {
        m_lastOpenedWorld = kNullGUID;
    }
}

void Project::onScenePlayStateChanged(EVENT_HANDLER(Scene))
{
    EVENT_ATTRS(Scene, ScenePlayStateChangedArgs);

    auto oldState = args->oldState;
    auto newState = args->newState;

    // going from editor to play mode
    if (oldState == PS_EDITOR && (newState == PS_PLAYING || newState == PS_PAUSED))
    {
        auto *oldWorld = m_scene.GetActiveWorld( );

        m_worldCache = ecs::WorldSerializer::Serialize( oldWorld );

        auto *playWorld = ecs::WorldSerializer::Deserialize( m_worldCache );

        playWorld->GetSettings( )->GetComponent<ecs::WorldConfig>( )->SetInEditorMode( false );

        m_scene.SetActiveWorld( ecs::World::Handle( playWorld ) );

        m_scene.LoadConfiguredSystems( );
    }
    // play mode to editor
    else if ((oldState == PS_PLAYING || oldState == PS_PAUSED) && newState == PS_EDITOR)
    {
        m_scene.GetScreenManager( ).ClearScreens( );

        auto *cachedWorld = ecs::WorldSerializer::Deserialize( m_worldCache );

        m_scene.SetActiveWorld( ecs::World::Handle( cachedWorld ) );

        cachedWorld->GetSettings( )->GetComponent<ecs::WorldConfig>( )->SetInEditorMode( true );
    }
    // switching between playing and paused
    else
    {
        m_scene.GetActiveWorld( )->GetSettings( )->GetComponent<ecs::WorldConfig>( )->SetInEditorMode( 
            newState == PS_PAUSED
        );
    }
}

void Project::onResourceModified(EVENT_HANDLER(rp::ResourcePipelineManager))
{
    EVENT_ATTRS(rp::ResourcePipelineManager, rp::ResourceChangeArgs);

    m_scene.GetResourceManager( ).ReloadIfCached( args->resource->GetGUID( ) );

    SceneResourceModifiedArgs e( args->resource->GetGUID( ) );

    m_scene.Dispatch( SCENE_RESOURCE_MODIFIED, &e );
}
