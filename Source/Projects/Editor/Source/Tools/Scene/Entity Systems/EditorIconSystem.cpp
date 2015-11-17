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
}

void EditorIconSystem::OnRemove(void)
{
    m_world->Listener( this )
        .Off(ecs::WORLD_ENTITY_COMPONENT_ADDED, &EditorIconSystem::onIconAdd );
}

void EditorIconSystem::onIconAdd(EVENT_HANDLER(ecs::World))
{
    EVENT_ATTRS(ecs::World, ecs::ComponentEventArgs);

    auto comp = args->component;

    if (args->entity->HasComponent<EditorIcon>( ))
        return;

    // if the object added was a selected component
    if (comp->Is<ecs::Light>( ))
    {
        args->entity->AddComponent<EditorIcon>( )->SetIcon( "Sun" );
    }
    else if (comp->Is<ecs::Camera>( ))
    {
        args->entity->AddComponent<EditorIcon>( )->SetIcon( "CameraIcon" );
    }
}