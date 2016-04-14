﻿/* ----------------------------------------------------------------------------
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

#include "Editor.h"
#include "Project.h"
#include "ComponentUtils.h"

#include <SelectedComponent.h>

using namespace ursine;

namespace
{
    namespace gameUIEvents
    {
        extern const char* const UI_HealthComponentStats;
    }

    namespace channel
    {
        const auto EntityManager = "EntityManager";
    }

    namespace events
    {
        namespace entity
        {
            const auto RefreshEntities = "RefreshEntities";
            const auto Added = "EntityAdded";
            const auto Removed = "EntityRemoved";
            const auto NameChanged = "EntityNameChanged";
            const auto ParentChanged = "EntityParentChanged";
            const auto VisibilityChanged = "EntityEditorVisibilityChanged";
        }

        namespace component
        {
            const auto Added = "ComponentAdded";
            const auto Removed = "ComponentRemoved";
            const auto Changed = "ComponentChanged";
            const auto ArrayInserted = "ComponentArrayInserted";
            const auto ArraySet = "ComponentArraySet";
            const auto ArrayRemove = "ComponentArrayRemove";
        }
    }
}

EditorEntityManager::EditorEntityManager(Project *project)
    : m_editor( GetCoreSystem( Editor ) )
    , m_project( project )
    , m_activeWorld( nullptr )
{
    m_project->GetScene( ).Listener( this )
        .On( SCENE_WORLD_CHANGED, &EditorEntityManager::onSceneActiveWorldChanged );
}

///////////////////////////////////////////////////////////////////////////////

EditorEntityManager::~EditorEntityManager(void)
{
    m_project->GetScene( ).Listener( this )
        .Off( SCENE_WORLD_CHANGED, &EditorEntityManager::onSceneActiveWorldChanged );

    clearWorldEvents( m_activeWorld );
}

///////////////////////////////////////////////////////////////////////////////

void EditorEntityManager::initWorldEvents(ecs::World *world)
{
    if (!world)
        return;

    world->Listener( this )
        .On( ecs::WORLD_ENTITY_ADDED, &EditorEntityManager::onEntityAdded )
        .On( ecs::WORLD_ENTITY_REMOVED, &EditorEntityManager::onEntityRemoved )
        .On( ecs::WORLD_EDITOR_ENTITY_NAME_CHANGED, &EditorEntityManager::onEntityNameChanged )
        .On( ecs::WORLD_EDITOR_ENTITY_PARENT_CHANGED, &EditorEntityManager::onEntityParentChanged )
        .On( ecs::WORLD_EDITOR_ENTITY_VISIBILITY_CHANGED, &EditorEntityManager::onEntityVisibilityChanged )
        .On( ecs::WORLD_ENTITY_COMPONENT_ADDED, &EditorEntityManager::onComponentAdded )
        .On( ecs::WORLD_ENTITY_COMPONENT_REMOVED, &EditorEntityManager::onComponentRemoved )
        .On( ecs::WORLD_EDITOR_ENTITY_COMPONENT_CHANGED, &EditorEntityManager::onComponentChanged )
        .On( ecs::WORLD_EDITOR_COMPONENT_ARRAY_MODIFIED, &EditorEntityManager::onComponentArrayModified );
}

///////////////////////////////////////////////////////////////////////////////

void EditorEntityManager::clearWorldEvents(ecs::World *world)
{
    if (!world)
        return;

    world->Listener( this )
        .Off( ecs::WORLD_ENTITY_ADDED, &EditorEntityManager::onEntityAdded )
        .Off( ecs::WORLD_ENTITY_REMOVED, &EditorEntityManager::onEntityRemoved )
        .Off( ecs::WORLD_EDITOR_ENTITY_NAME_CHANGED, &EditorEntityManager::onEntityNameChanged )
        .Off( ecs::WORLD_EDITOR_ENTITY_PARENT_CHANGED, &EditorEntityManager::onEntityParentChanged )
        .Off( ecs::WORLD_EDITOR_ENTITY_VISIBILITY_CHANGED, &EditorEntityManager::onEntityVisibilityChanged )
        .Off( ecs::WORLD_ENTITY_COMPONENT_ADDED, &EditorEntityManager::onComponentAdded )
        .Off( ecs::WORLD_ENTITY_COMPONENT_REMOVED, &EditorEntityManager::onComponentRemoved )
        .Off( ecs::WORLD_EDITOR_ENTITY_COMPONENT_CHANGED, &EditorEntityManager::onComponentChanged )
        .Off( ecs::WORLD_EDITOR_COMPONENT_ARRAY_MODIFIED, &EditorEntityManager::onComponentArrayModified );
}

///////////////////////////////////////////////////////////////////////////////

void EditorEntityManager::onSceneActiveWorldChanged(EVENT_HANDLER(Scene))
{
    EVENT_ATTRS(Scene, SceneWorldChangedArgs);

    clearWorldEvents( args->oldWorld.get( ) );

    m_activeWorld = sender->GetActiveWorld( );

    initWorldEvents( m_activeWorld );
}

///////////////////////////////////////////////////////////////////////////////

void EditorEntityManager::onEntityAdded(EVENT_HANDLER(ecs::World))
{
    EVENT_ATTRS(ecs::World, ecs::EntityEventArgs);

    if (m_project->GetScene( ).GetPlayState( ) == PS_PLAYING &&
        !args->entity->HasComponent<ecs::Selected>( ))
        return;

    Json message = Json::object {
        { "uniqueID", static_cast<int>( args->entity->GetID( ) ) }
    };

    m_editor->GetMainWindow( ).GetUI( )->Message(
        UI_CMD_BROADCAST, 
        channel::EntityManager, 
        events::entity::Added, 
        message 
    );
}

///////////////////////////////////////////////////////////////////////////////

void EditorEntityManager::onEntityRemoved(EVENT_HANDLER(ecs::World))
{
    EVENT_ATTRS(ecs::World, ecs::EntityEventArgs);

    if (m_project->GetScene( ).GetPlayState( ) == PS_PLAYING &&
        !args->entity->HasComponent<ecs::Selected>( ))
        return;

    Json message = Json::object {
        { "uniqueID", static_cast<int>( args->entity->GetID( ) ) }
    };

    m_editor->GetMainWindow( ).GetUI( )->Message(
        UI_CMD_BROADCAST, 
        channel::EntityManager, 
        events::entity::Removed, 
        message 
    );
}

///////////////////////////////////////////////////////////////////////////////

void EditorEntityManager::onEntityNameChanged(EVENT_HANDLER(ursine::ecs::World))
{
    EVENT_ATTRS(ecs::World, ecs::EditorEntityNameChangedArgs);

    if (m_project->GetScene( ).GetPlayState( ) == PS_PLAYING &&
        !args->entity->HasComponent<ecs::Selected>( ))
        return;

    Json message = Json::object {
        { "uniqueID", static_cast<int>( args->entity->GetID( ) ) },
        { "name", args->newName },
    };

    m_editor->GetMainWindow( ).GetUI( )->Message(
        UI_CMD_BROADCAST, 
        channel::EntityManager, 
        events::entity::NameChanged,
        message
    );
}

///////////////////////////////////////////////////////////////////////////////

void EditorEntityManager::onEntityParentChanged(EVENT_HANDLER(ecs::Entity))
{
    EVENT_ATTRS(ecs::Entity, ecs::ParentChangedArgs);

    if (m_project->GetScene( ).GetPlayState( ) == PS_PLAYING &&
        !sender->HasComponent<ecs::Selected>( ))
        return;

    Json oldUniqueID, newUniqueID;

    if (!args->oldParent)
        oldUniqueID = nullptr;
    else
        oldUniqueID = static_cast<int>( args->oldParent->GetID( ) );

    if (!args->newParent)
        newUniqueID = nullptr;
    else
        newUniqueID = static_cast<int>( args->newParent->GetID( ) );

    Json message = Json::object {
        { "uniqueID", static_cast<int>( sender->GetID( ) ) },
        { "oldParent", oldUniqueID },
        { "newParent", newUniqueID }
    };

    m_editor->GetMainWindow( ).GetUI( )->Message(
        UI_CMD_BROADCAST, 
        channel::EntityManager, 
        events::entity::ParentChanged,
        message
    );
}

///////////////////////////////////////////////////////////////////////////////

void EditorEntityManager::onEntityVisibilityChanged(EVENT_HANDLER(ecs::World))
{
    EVENT_ATTRS(ecs::World, ecs::EntityEventArgs);

    if (m_project->GetScene( ).GetPlayState( ) == PS_PLAYING && 
        !args->entity->HasComponent<ecs::Selected>( ))
        return;

    Json message = Json::object {
        { "uniqueID", static_cast<int>( args->entity->GetID( ) ) },
        { "visible", args->entity->IsVisibleInEditor( ) },
    };

    m_editor->GetMainWindow( ).GetUI( )->Message(
        UI_CMD_BROADCAST, 
        channel::EntityManager, 
        events::entity::VisibilityChanged,
        message
    );
}

///////////////////////////////////////////////////////////////////////////////

void EditorEntityManager::onComponentAdded(EVENT_HANDLER(ecs::World))
{
    EVENT_ATTRS(ecs::World, ecs::ComponentEventArgs);

    if (args->component == nullptr || !args->entity)
        return;

    auto selected = args->entity->HasComponent<ecs::Selected>( );

    if (!selected || m_project->GetScene( ).GetPlayState( ) == PS_PLAYING)
        return;

    auto component = 
        meta::Variant( args->component, meta::variant_policy::WrapObject( ) );

    Json message = Json::object {
        { "uniqueID", static_cast<int>( args->entity->GetID( ) ) },
        { "component", args->component->GetType( ).GetName( ) },
        // note: false is to ensure no serialization hooks are called
        { "value", component.GetType( ).SerializeJson( component, false ) },
        { "buttons", InspectComponentButtons( component ) }
    };

    m_editor->GetMainWindow( ).GetUI( )->Message(
        UI_CMD_BROADCAST, 
        channel::EntityManager, 
        events::component::Added,
        message
    );
}

///////////////////////////////////////////////////////////////////////////////

void EditorEntityManager::onComponentRemoved(EVENT_HANDLER(ecs::World))
{
    EVENT_ATTRS(ecs::World, ecs::ComponentEventArgs);

    auto selected = args->entity->HasComponent<ecs::Selected>( );

    if (!selected || m_project->GetScene( ).GetPlayState( ) == PS_PLAYING)
        return;

    Json message = Json::object {
        { "uniqueID", static_cast<int>( args->entity->GetID( ) ) },
        { "component", args->component->GetType( ).GetName( ) },
    };

    m_editor->GetMainWindow( ).GetUI( )->Message(
        UI_CMD_BROADCAST, 
        channel::EntityManager, 
        events::component::Removed,
        message
    );
}

///////////////////////////////////////////////////////////////////////////////

void EditorEntityManager::onComponentChanged(EVENT_HANDLER(ecs::World))
{
    EVENT_ATTRS(ecs::World, ecs::EditorComponentChangedArgs);

    auto selected = args->entity->HasComponent<ecs::Selected>( );

    if (!selected || m_project->GetScene( ).GetPlayState( ) == PS_PLAYING)
        return;

    auto componentType = args->component->GetType( );
    auto field = componentType.GetField( args->field );

    UAssert( field.IsValid( ),
        "Notifying change of unknown field.\n"
        "Component: %s\n"
        "Field: %s",
        componentType.GetName( ).c_str( ),
        args->field.c_str( )
    );

    auto invokeHook = field.GetMeta( ).GetProperty<ForceSerializationHook>( ) != nullptr;

    Json message = Json::object {
        { "uniqueID", static_cast<int>( args->entity->GetID( ) ) },
        { "component", componentType.GetName( ) },
        { "field", args->field },
        { "value", args->value.GetType( ).SerializeJson( args->value, invokeHook ) }
    };

    m_editor->GetMainWindow( ).GetUI( )->Message(
        UI_CMD_BROADCAST, 
        channel::EntityManager, 
        events::component::Changed,
        message
    );
}

///////////////////////////////////////////////////////////////////////////////

void EditorEntityManager::onComponentArrayModified(EVENT_HANDLER(ecs::World))
{
    EVENT_ATTRS(ecs::World, ecs::EditorComponentArrayModfiedArgs);

    auto selected = args->entity->HasComponent<ecs::Selected>( );

    if (!selected || m_project->GetScene( ).GetPlayState( ) == PS_PLAYING)
        return;

    #if defined(CONFIG_DEBUG)
        UAssert( args->component->GetType( ).GetField( args->field ).IsValid( ),
            "Notifying change of unknown field.\n"
            "Component: %s\n"
            "Field: %s",
            args->component->GetType( ).GetName( ).c_str( ),
            args->field.c_str( )
        );
    #endif

    Json::object message = Json::object {
        { "uniqueID", static_cast<int>( args->entity->GetID( ) ) },
        { "component", args->component->GetType( ).GetName( ) },
        { "field", args->field },
        { "index", static_cast<int>( args->modification.index ) }
    };

    if (args->modification.value.IsValid( ))
    {
        // note: false is to ensure no serialization hooks are called
        message[ "value" ] = args->modification.value.GetType( ).SerializeJson( args->modification.value, false );
    }
    else
    {
        message[ "value" ] = nullptr;
    }

    static std::unordered_map<ArrayModifyAction, std::string> actionToType
    {
        { AMODIFY_INSERT, events::component::ArrayInserted },
        { AMODIFY_SET, events::component::ArraySet },
        { AMODIFY_REMOVE, events::component::ArrayRemove }
    };

    m_editor->GetMainWindow( ).GetUI( )->Message(
        UI_CMD_BROADCAST, 
        channel::EntityManager, 
        actionToType[ args->modification.action ],
        message
    );
}