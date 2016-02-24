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

#include <WorldData.h>

#include <LightComponent.h>
#include <Color.h>

using namespace ursine;

namespace
{
    const auto kResourcesBuildDirectory = "Resources";
}

Project::Project(void)
    : m_entityManager( nullptr )
    , m_lastOpenedWorld( GUIDNullGenerator( )( ) )
{
    m_scene.Listener( this )
        .On( SCENE_WORLD_CHANGED, &Project::onSceneWorldChanged );
}

Project::~Project(void)
{
    m_scene.Listener( this )
        .Off( SCENE_WORLD_CHANGED, &Project::onSceneWorldChanged );

    delete m_entityManager;
}

const ProjectConfig &Project::GetConfig(void) const
{
    return m_config;
}

rp::ResourcePipelineManager &Project::GetResourcePipeline(void)
{
    return m_resourcePipeline;
}

Scene &Project::GetScene(void)
{
    return m_scene;
}

void Project::SetEmptyScene(void)
{
    auto world = std::make_shared<ecs::World>( );

    // make sure there is an owner before creating entities
    world->SetOwner( &m_scene );

    auto *univLight = world->CreateEntity( "Global Light" );
    {
        auto *component = univLight->AddComponent<ecs::Light>( );

        univLight->GetTransform( )->SetLocalPosition( { 0.0f, 60.0f, 0.0f } );

        component->SetLightType( ecs::LightType::Directional );
        component->SetRadius( 40.0f );
        component->SetColor( Color( 0.5f, 0.5f, 0.5f, 1.0f ) );
    }

    world->DispatchLoad( );

    m_scene.SetActiveWorld( world );
}

const ursine::GUID &Project::GetLastOpenedWorld(void)
{
    return m_lastOpenedWorld;
}

void Project::initialize(const ProjectConfig &config)
{
    m_config = config;

    rp::ResourcePipelineConfig resourceConfig;

    resourceConfig.resourceDirectory = config.rootDirectory / config.resourceDirectory;

    resourceConfig.buildDirectory = config.rootDirectory / config.buildDirectory;
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
    m_entityManager = new EditorEntityManager( this );

    auto world = startingWorld.Load<resources::WorldData>( );

    if (!world)
        return SetEmptyScene( );

    m_scene.SetActiveWorld( startingWorld );
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
        m_lastOpenedWorld = GUIDNullGenerator( )( );
    }
}