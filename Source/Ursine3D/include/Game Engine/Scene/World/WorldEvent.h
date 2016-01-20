/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** WorldEvent.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "Entity.h"
#include "Component.h"

namespace ursine
{
    namespace ecs
    {
        enum WorldEventType
        {
            // The world is being updated
            WORLD_UPDATE,
            // The world is being rendered
            WORLD_RENDER,

            // An entity was created
            WORLD_ENTITY_ADDED,
            // An entity was removed
            WORLD_ENTITY_REMOVED,

            // A component has been added to an entity
            WORLD_ENTITY_COMPONENT_ADDED,
            // A component has been removed from an entity
            WORLD_ENTITY_COMPONENT_REMOVED,

            // The screen that owns this world's focus state changed
            WORLD_SCREEN_FOCUS_CHANGED,

        #if defined(URSINE_WITH_EDITOR)

            // Update event called specifically for editor systems
            WORLD_EDITOR_UPDATE,
            // An entity's name changed
            WORLD_EDITOR_ENTITY_NAME_CHANGED = 0x100,
            // An entity's parent has changed
            WORLD_EDITOR_ENTITY_PARENT_CHANGED,
            // A component's field has changed
            WORLD_EDITOR_ENTITY_COMPONENT_CHANGED,
            // A component's array field has been modified
            WORLD_EDITOR_COMPONENT_ARRAY_MODIFIED

        #endif
        };

        struct WorldEventArgs : EventArgs
        {
            WorldEventType type;

            WorldEventArgs(WorldEventType type)
                : type( type ) { }
        };

        struct EntityEventArgs : WorldEventArgs
        {
            Entity *entity;

            EntityEventArgs(WorldEventType type, Entity *entity)
                : WorldEventArgs( type )
                , entity( entity ) { }
        };

        struct ComponentEventArgs : WorldEventArgs
        {
            Entity *entity;
            Component *component;

            ComponentEventArgs(WorldEventType type, Entity *entity, Component *component)
                : WorldEventArgs( type )
                , entity( entity )
                , component( component ) { }
        };

        struct ComponentRemovedEventArgs : ComponentEventArgs
        {
            ComponentTypeMask oldTypeMask;

            ComponentRemovedEventArgs(WorldEventType type, Entity *entity, Component *component, ComponentTypeMask oldTypeMask)
                : ComponentEventArgs( type, entity, component )
                , oldTypeMask( oldTypeMask ) { }
        };

        struct ScreenFocusArgs : WorldEventArgs
        {
            bool focused;

            ScreenFocusArgs(WorldEventType type, bool focused)
                : WorldEventArgs( type )
                , focused( focused ) { }
        };

    #if defined(URSINE_WITH_EDITOR)

        struct EditorEntityNameChangedArgs : EntityEventArgs
        {
            std::string newName;

            EditorEntityNameChangedArgs(WorldEventType type, Entity *entity, const std::string &newName)
                : EntityEventArgs( type, entity )
                , newName( newName ) { }
        };

        struct EditorComponentChangedArgs : ComponentEventArgs
        {
            const Component *component;
            const std::string field;
            const meta::Variant value;

            EditorComponentChangedArgs(
                WorldEventType type,
                Entity *entity,
                Component *component,
                const std::string &field,
                const meta::Variant &value
            )
                : ComponentEventArgs( type, entity, component )
                , component( component )
                , field( field )
                , value( value ) { }
        };

        struct EditorComponentArrayModfiedArgs : ArrayModificationArgs
        {
            std::string field;

            EditorComponentArrayModfiedArgs(const ArrayModificationArgs &args, const std::string &field)
                : ArrayModificationArgs( args ) 
                , field( field ) { }
        };

    #endif
    }
}