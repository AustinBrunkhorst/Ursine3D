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

ENTITY_SYSTEM_DEFINITION( EditorIconSystem );

using namespace ursine;

EditorIconSystem::EditorIconSystem(ecs::World* world)
    : EntitySystem( world )
{
    
}

void EditorIconSystem::OnInitialize(void)
{
    m_world->Listener( this )
        .On( ecs::WORLD_ENTITY_COMPONENT_ADDED, &EditorIconSystem::onIconAdd );

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
        .Off( ecs::WORLD_ENTITY_COMPONENT_ADDED, &EditorIconSystem::onIconAdd );
}

void EditorIconSystem::onIconAdd(EVENT_HANDLER(ecs::World))
{
    EVENT_ATTRS(ecs::World, ecs::ComponentEventArgs);

    auto comp = args->component;

    // if the object added was a selected component
    if (comp->Is<ecs::Light>( ))
    {
        if ( !args->entity->HasComponent<EditorIcon>( ) )
            args->entity->AddComponent<EditorIcon>( );
        args->entity->GetComponent<EditorIcon>( )->SetIcon( "Sun" );
    }
    else if (comp->Is<ecs::Camera>( ))
    {
        if ( !args->entity->HasComponent<EditorIcon>( ) )
            args->entity->AddComponent<EditorIcon>( );
        args->entity->GetComponent<EditorIcon>( )->SetIcon( "CameraIcon" );
    }
}