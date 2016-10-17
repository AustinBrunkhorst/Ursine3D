/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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

            // For when the volume is changed
            // @Austin there has to be a better place for this in the code
            // TODO: Find an alternative for this
            WORLD_VOLUME_CHANGE,

        #if defined(URSINE_WITH_EDITOR)

            // Update event called specifically for editor systems
            WORLD_EDITOR_UPDATE,
            // Render event called specifically for editor systems
            WORLD_EDITOR_RENDER,

            // An entity's name changed
            WORLD_EDITOR_ENTITY_NAME_CHANGED = 0x100,
            // An entity's parent has changed
            WORLD_EDITOR_ENTITY_PARENT_CHANGED,
            // An entity's visibilty to the editor has changed
            WORLD_EDITOR_ENTITY_VISIBILITY_CHANGED,
            // A component's field has changed
            WORLD_EDITOR_ENTITY_COMPONENT_CHANGED,
            // A component's array field has been modified
            WORLD_EDITOR_COMPONENT_ARRAY_MODIFIED,

        #endif

            // Starting index for events defined out of engine
            WORLD_EVENT_USER
        };

        struct WorldEventArgs : EventArgs
        {
            WorldEventType type;

            WorldEventArgs(WorldEventType type)
                : type( type ) { }
        };

        struct EntityEventArgs : WorldEventArgs
        {
            EntityHandle entity;

            EntityEventArgs(WorldEventType type, const EntityHandle &entity)
                : WorldEventArgs( type )
                , entity( entity ) { }
        };

        struct ComponentEventArgs : WorldEventArgs
        {
            EntityHandle entity;
            Component *component;

            ComponentEventArgs(WorldEventType type, const EntityHandle &entity, Component *component)
                : WorldEventArgs( type )
                , entity( entity )
                , component( component ) { }
        };

        struct ComponentRemovedEventArgs : ComponentEventArgs
        {
            ComponentTypeMask oldTypeMask;

            ComponentRemovedEventArgs(WorldEventType type, const EntityHandle &entity, Component *component, ComponentTypeMask oldTypeMask)
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

        struct VolumeChangeArgs : WorldEventArgs
        {
            float volume;
            std::string outputType;

            VolumeChangeArgs(float volume, const std::string &outputType)
                : WorldEventArgs( WORLD_VOLUME_CHANGE )
                , volume( volume )
                , outputType( outputType ) { }
        };

    #if defined(URSINE_WITH_EDITOR)

        struct EditorEntityNameChangedArgs : EntityEventArgs
        {
            std::string newName;

            EditorEntityNameChangedArgs(WorldEventType type, const EntityHandle &entity, const std::string &newName)
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
                const EntityHandle &entity,
                Component *component,
                const std::string &field,
                const meta::Variant &value
            )
                : ComponentEventArgs( type, entity, component )
                , component( component )
                , field( field )
                , value( value ) { }
        };

        struct EditorComponentArrayModfiedArgs : ComponentEventArgs
        {
            const ArrayModificationArgs &modification;
            std::string field;

            EditorComponentArrayModfiedArgs(const ArrayModificationArgs &args, const EntityHandle &entity, Component *component, const std::string &field)
                : ComponentEventArgs( WORLD_EDITOR_COMPONENT_ARRAY_MODIFIED, entity, component )
                , modification( args )
                , field( field ) { }
        };

    #endif
    }
}