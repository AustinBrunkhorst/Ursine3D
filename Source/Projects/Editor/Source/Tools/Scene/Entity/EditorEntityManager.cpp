/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EditorEntityManager.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "EditorEntityManager.h"
#include "SelectedComponent.h"

using namespace ursine;

namespace
{
    namespace channel
    {
        const auto SceneManager = "SceneManager";
        const auto EntityManager = "EntityManager";
    }

    namespace events
    {
        namespace scene
        {
            const auto Reset = "Reset";
        }

        namespace entity
        {
            const auto Added = "EntityAdded";
            const auto Removed = "EntityRemoved";
            const auto NameChanged = "EntityNameChanged";
            const auto ParentChanged = "EntityParentChanged";
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
    , m_world( nullptr )
{

}

EditorEntityManager::~EditorEntityManager(void)
{
    clearWorld( m_world );
}

void EditorEntityManager::SetWorld(ecs::World::Handle world)
{
    clearWorld( m_world );

    world->Listener( this )
        .On( ecs::WORLD_ENTITY_ADDED, &EditorEntityManager::onEntityAdded )
        .On( ecs::WORLD_ENTITY_REMOVED, &EditorEntityManager::onEntityRemoved )
        .On( ecs::WORLD_EDITOR_ENTITY_NAME_CHANGED, &EditorEntityManager::onEntityNameChanged )
        .On( ecs::WORLD_EDITOR_ENTITY_PARENT_CHANGED, &EditorEntityManager::onEntityParentChanged )
        .On( ecs::WORLD_ENTITY_COMPONENT_ADDED, &EditorEntityManager::onComponentAdded )
        .On( ecs::WORLD_ENTITY_COMPONENT_REMOVED, &EditorEntityManager::onComponentRemoved )
        .On( ecs::WORLD_EDITOR_ENTITY_COMPONENT_CHANGED, &EditorEntityManager::onComponentChanged );

    m_world = world;

    Json data;

    m_project->GetUI( )->Message(
        UI_CMD_BROADCAST, 
        channel::SceneManager, 
        events::scene::Reset,
        data
    );
}

void EditorEntityManager::clearWorld(ecs::World::Handle world)
{
    if (!world)
        return;

    world->Listener( this )
        .Off( ecs::WORLD_ENTITY_ADDED, &EditorEntityManager::onEntityAdded )
        .Off( ecs::WORLD_ENTITY_REMOVED, &EditorEntityManager::onEntityRemoved )
        .Off( ecs::WORLD_EDITOR_ENTITY_NAME_CHANGED, &EditorEntityManager::onEntityNameChanged )
        .Off( ecs::WORLD_EDITOR_ENTITY_PARENT_CHANGED, &EditorEntityManager::onEntityParentChanged )
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

void EditorEntityManager::onEntityParentChanged(EVENT_HANDLER(ecs::Entity))
{
    EVENT_ATTRS(ecs::Entity, ecs::ParentChangedArgs);

    Json oldUniqueID, newUniqueID;

    if (args->oldParent == -1)
        oldUniqueID = nullptr;
    else
        oldUniqueID = static_cast<int>( args->oldParent );

    if (args->newParent == -1)
        newUniqueID = nullptr;
    else
        newUniqueID = static_cast<int>( args->newParent );

    Json message = Json::object {
        { "uniqueID", static_cast<int>( sender->GetUniqueID( ) ) },
        { "oldParent", oldUniqueID },
        { "newParent", newUniqueID }
    };

    m_project->GetUI( )->Message(
        UI_CMD_BROADCAST, 
        channel::EntityManager, 
        events::entity::ParentChanged,
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
    #if defined(CONFIG_DEBUG)
        UAssert( args->component->GetType( ).GetField( args->field ).IsValid( ),
            "Notifying change of unknown field.\n"
            "Component: %s\n"
            "Field: %s",
            args->component->GetType( ).GetName( ).c_str( ),
            args->field.c_str( )
        );
    #endif

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
