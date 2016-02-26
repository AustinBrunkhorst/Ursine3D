/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EditorIconSystem.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "EditorIconSystem.h"
#include "EditorIconComponent.h"

#include <LightComponent.h>
#include <CameraComponent.h>
#include "GUID.h"
#include "Scene.h"
#include <Core/Resources/Types/TextureData.h>

ENTITY_SYSTEM_DEFINITION( EditorIconSystem );

using namespace ursine;

EditorIconSystem::EditorIconSystem(ecs::World* world)
    : EntitySystem( world )
{
    
}

void EditorIconSystem::OnInitialize(void)
{
    m_world->Listener( this )
        .On( ecs::WORLD_ENTITY_COMPONENT_ADDED, &EditorIconSystem::onIconAdd )
        .On( ecs::WORLD_EDITOR_ENTITY_COMPONENT_CHANGED, &EditorIconSystem::onLightTypeChange );

    // Get all entities already in the world
    auto cams = m_world->GetEntitiesFromFilter( ecs::Filter( ).All<ecs::Camera>( ) );

    for (auto &cam : cams)
    {
        ecs::ComponentEventArgs args( 
            ecs::WorldEventType::WORLD_ENTITY_COMPONENT_ADDED, 
            cam, cam->GetComponent<ecs::Camera>( ) 
        );

        onIconAdd( m_world, &args );
    }

    auto lights = m_world->GetEntitiesFromFilter( ecs::Filter( ).All<ecs::Light>( ) );

    for (auto &light : lights)
    {
        ecs::ComponentEventArgs args( 
            ecs::WorldEventType::WORLD_ENTITY_COMPONENT_ADDED, 
            light, light->GetComponent<ecs::Light>( )
        );

        onIconAdd( m_world, &args );
    }
}

void EditorIconSystem::OnRemove(void)
{
    m_world->Listener( this )
        .Off( ecs::WORLD_ENTITY_COMPONENT_ADDED, &EditorIconSystem::onIconAdd )
        .Off( ecs::WORLD_EDITOR_ENTITY_COMPONENT_CHANGED, &EditorIconSystem::onLightTypeChange );
}

void EditorIconSystem::onIconAdd(EVENT_HANDLER(ecs::World))
{
    EVENT_ATTRS(ecs::World, ecs::ComponentEventArgs);

    auto comp = args->component;
    auto entity = args->entity;

    // if the object added was a selected component
    if (comp->Is<ecs::Light>( ))
    {
        /*if ( !args->entity->HasComponent<EditorIcon>( ) )
            args->entity->AddComponent<EditorIcon>( );*/

        //auto testReference = m_world->GetOwner()->GetResourceManager().CreateReference( GUIDStringGenerator()("11361b8e-1ca2-4754-a0d3-ab83f60adf22") );
        //auto *texHandle = testReference.Load<resources::TextureData>( );

        //args->entity->GetComponent<EditorIcon>()->SetIcon( texHandle->GetTextureHandle() );
    }
    else if (comp->Is<ecs::Camera>( ))
    {
        /*if ( !args->entity->HasComponent<EditorIcon>( ) )
            args->entity->AddComponent<EditorIcon>( );*/

        //auto testReference = m_world->GetOwner()->GetResourceManager().CreateReference(GUIDStringGenerator()("0dca7489-2a8d-4fb0-80be-3e2f3ce5c235"));
        //auto *texHandle = testReference.Load<resources::TextureData>();

        //args->entity->GetComponent<EditorIcon>()->SetIcon(texHandle->GetTextureHandle());
    }
}

void EditorIconSystem::onLightTypeChange(EVENT_HANDLER(ecs::World))
{
    EVENT_ATTRS(ecs::World, ecs::EditorComponentChangedArgs);

    if (args->component->Is<ecs::Light>( ) && args->field == "type")
    {
        auto type = static_cast<ecs::LightType>( args->value.ToInt( ) );
        auto icon = args->entity->GetComponent<EditorIcon>( );

        if (!icon)
            return;

        setLightIcon( type, icon );
    }
}

void EditorIconSystem::setLightIcon(ursine::ecs::LightType type, EditorIcon* icon)
{
    switch (type)
    {
    case ecs::LightType::Directional:
        //icon->SetIcon( "DirectionalLightIcon" );
        break;

    case ecs::LightType::Point:
        //icon->SetIcon( "PointLightIcon" );
        break;

    case ecs::LightType::Spot:
        //icon->SetIcon( "SpotLightIcon" );
        break;
    }
}
