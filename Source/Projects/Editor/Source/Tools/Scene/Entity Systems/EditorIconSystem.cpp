#include "Precompiled.h"
#include "EditorIconSystem.h"
#include <Tools/Scene/Components/EditorIconComponent.h>
#include <Game Engine/Scene/Component/Native Components/LightComponent.h>
#include <Game Engine/Scene/Component/Native Components/CameraComponent.h>

ENTITY_SYSTEM_DEFINITION(EditorIconSystem);

EditorIconSystem::EditorIconSystem(ursine::ecs::World* world)
    : EntitySystem(world)
{
    
}

void EditorIconSystem::OnInitialize()
{
    m_world->Listener(this)
        .On( ursine::ecs::WorldEventType::WORLD_ENTITY_COMPONENT_ADDED, &EditorIconSystem::onIconAdd);
}

void EditorIconSystem::OnRemove()
{
    m_world->Listener(this)
        .Off(ursine::ecs::WorldEventType::WORLD_ENTITY_COMPONENT_ADDED, &EditorIconSystem::onIconAdd);
}

void EditorIconSystem::onIconAdd(EVENT_HANDLER(ursine::ecs::World))
{
    EVENT_ATTRS(ursine::ecs::World, ursine::ecs::ComponentEventArgs);

    auto comp = args->component;

    //if the object added was a selected component
    if (comp->Is<ursine::ecs::Light>())
    {
        args->component->GetOwner()->AddComponent<EditorIcon>()->SetIcon("Sun");
    }
    else if (comp->Is<ursine::ecs::Camera>())
    {
        args->component->GetOwner()->AddComponent<EditorIcon>()->SetIcon("Camera");
    }
}