#include "Precompiled.h"

#include "EditorEntityManager.h"
#include "SelectedComponent.h"

using namespace ursine;

namespace
{
    namespace channel
    {
        const auto EntityManager = "EntityManager";
    }

    namespace events
    {
        namespace entity
        {
            const auto Added = "EntityAdded";
            const auto Removed = "EntityRemoved";
            const auto NameChanged = "EntityNameChanged";
        }

        namespace component
        {
            const auto Added = "ComponentAdded";
            const auto Removed = "ComponentRemoved";
            const auto Changed = "ComponentChanged";
        }
    }
}

EditorEntityManager::EditorEntityManager(Project *project)
    : m_project( project )
{
    m_project->GetScene( ).GetWorld( ).Listener( this )
        .On( ecs::WORLD_ENTITY_ADDED, &EditorEntityManager::onEntityAdded )
        .On( ecs::WORLD_ENTITY_REMOVED, &EditorEntityManager::onEntityRemoved )
        .On( ecs::WORLD_EDITOR_ENTITY_NAME_CHANGED, &EditorEntityManager::onEntityNameChanged )
        .On( ecs::WORLD_ENTITY_COMPONENT_ADDED, &EditorEntityManager::onComponentAdded )
        .On( ecs::WORLD_ENTITY_COMPONENT_REMOVED, &EditorEntityManager::onComponentRemoved )
        .On( ecs::WORLD_EDITOR_ENTITY_COMPONENT_CHANGED, &EditorEntityManager::onComponentChanged );
}

EditorEntityManager::~EditorEntityManager(void)
{
    m_project->GetScene( ).GetWorld( ).Listener( this )
        .Off( ecs::WORLD_ENTITY_ADDED, &EditorEntityManager::onEntityAdded )
        .Off( ecs::WORLD_ENTITY_REMOVED, &EditorEntityManager::onEntityRemoved )
        .Off( ecs::WORLD_EDITOR_ENTITY_NAME_CHANGED, &EditorEntityManager::onEntityNameChanged )
        .Off( ecs::WORLD_ENTITY_COMPONENT_ADDED, &EditorEntityManager::onComponentAdded )
        .Off( ecs::WORLD_ENTITY_COMPONENT_REMOVED, &EditorEntityManager::onComponentRemoved )
        .Off( ecs::WORLD_EDITOR_ENTITY_COMPONENT_CHANGED, &EditorEntityManager::onComponentChanged );
}

void EditorEntityManager::onEntityAdded(EVENT_HANDLER(ecs::World))
{
    EVENT_ATTRS(ecs::World, ecs::EntityEventArgs);

    Json message = Json::object {
        { "uniqueID", static_cast<int>( args->entity->GetUniqueID( ) ) }
    };

    m_project->GetUI( )->Message(
        UI_CMD_BROADCAST, 
        channel::EntityManager, 
        events::entity::Added, 
        message 
    );
}

void EditorEntityManager::onEntityRemoved(EVENT_HANDLER(ecs::World))
{
    EVENT_ATTRS(ecs::World, ecs::EntityEventArgs);

    Json message = Json::object {
        { "uniqueID", static_cast<int>( args->entity->GetUniqueID( ) ) }
    };

    m_project->GetUI( )->Message(
        UI_CMD_BROADCAST, 
        channel::EntityManager, 
        events::entity::Removed, 
        message 
    );
}

void EditorEntityManager::onEntityNameChanged(EVENT_HANDLER(ursine::ecs::World))
{
    EVENT_ATTRS(ecs::World, ecs::EditorEntityNameChangedArgs);

    Json message = Json::object {
        { "uniqueID", static_cast<int>( args->entity->GetUniqueID( ) ) },
        { "name", args->newName },
    };

    m_project->GetUI( )->Message(
        UI_CMD_BROADCAST, 
        channel::EntityManager, 
        events::entity::NameChanged,
        message
    );
}

void EditorEntityManager::onComponentAdded(EVENT_HANDLER(ecs::World))
{
    EVENT_ATTRS(ecs::World, ecs::ComponentEventArgs);

    auto component = 
        meta::Variant( args->component, meta::variant_policy::WrapObject( ) );

    Json message = Json::object {
        { "uniqueID", static_cast<int>( args->entity->GetUniqueID( ) ) },
        { "component", args->component->GetType( ).GetName( ) },
        { "value", component.SerializeJson( ) }
    };

    m_project->GetUI( )->Message(
        UI_CMD_BROADCAST, 
        channel::EntityManager, 
        events::component::Added,
        message
    );
}

void EditorEntityManager::onComponentRemoved(EVENT_HANDLER(ecs::World))
{
    EVENT_ATTRS(ecs::World, ecs::ComponentEventArgs);

    Json message = Json::object {
        { "uniqueID", static_cast<int>( args->entity->GetUniqueID( ) ) },
        { "component", args->component->GetType( ).GetName( ) },
    };

    m_project->GetUI( )->Message(
        UI_CMD_BROADCAST, 
        channel::EntityManager, 
        events::component::Removed,
        message
    );
}

void EditorEntityManager::onComponentChanged(EVENT_HANDLER(ecs::World))
{
    EVENT_ATTRS(ecs::World, ecs::EditorComponentChangedArgs);

    if (args->entity->HasComponent<Selected>( ))
    {
        Json message = Json::object {
            { "uniqueID", static_cast<int>( args->entity->GetUniqueID( ) ) },
            { "component", args->component->GetType( ).GetName( ) },
            { "field", args->field },
            { "value", args->value.SerializeJson( ) }
        };

        m_project->GetUI( )->Message(
            UI_CMD_BROADCAST, 
            channel::EntityManager, 
            events::component::Changed,
            message
        );
    }
}
