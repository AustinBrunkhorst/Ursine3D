#pragma once

#include "Entity.h"
#include "Component.h"

namespace ursine
{
    namespace ecs
    {
        enum WorldEventType
        {
            WORLD_UPDATE,
            WORLD_RENDER,
            WORLD_ENTITY_ADDED,
            WORLD_ENTITY_REMOVED,
            WORLD_ENTITY_COMPONENT_ADDED,
            WORLD_ENTITY_COMPONENT_REMOVED
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
            const Component *component;

            ComponentEventArgs(WorldEventType type, Entity *entity, Component *component)
                : WorldEventArgs( type )
                , entity( entity )
                , component( component ) { }
        };
    }
}