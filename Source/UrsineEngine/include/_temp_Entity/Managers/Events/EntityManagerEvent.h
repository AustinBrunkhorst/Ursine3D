/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EntityManagerEvent.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "EventArgs.h"

namespace ursine
{
    namespace ecs
    {
        // forward declarations
        class Entity;
        class Component;

        enum EntityManagerEvent
        {
            EM_COMPONENT_ADDED,
            EM_COMPONENT_REMOVED,
            EM_ENTITY_ADDED,
            EM_ENTITY_REMOVED
        };

        struct EntityManagerArgs : public EventArgs
        {
            EntityManagerEvent event_type;

            EntityManagerArgs(EntityManagerEvent event_type)
                : event_type(event_type) {}
        };

        struct EntityEventArgs : public EntityManagerArgs
        {
            Entity *entity;

            EntityEventArgs(EntityManagerEvent event_type, Entity *entity)
                : EntityManagerArgs(event_type)
                , entity(entity) {}
        };

        struct ComponentEventArgs : public EntityManagerArgs
        {
            Entity *entity;
            const Component *component;

            ComponentEventArgs(EntityManagerEvent event_type, Entity *entity, Component *component)
                : EntityManagerArgs(event_type)
                , entity(entity)
                , component(component) {}
        };
    }
}