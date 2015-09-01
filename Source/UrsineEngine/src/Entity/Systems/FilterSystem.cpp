/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** FilterSystem.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/


#include "UrsinePrecompiled.h"

#include "FilterSystem.h"
#include "Entity.h"

namespace ursine
{
    namespace ecs
    {
        FilterSystem::FilterSystem(World *world, const Filter &filter, SystemPriority priority)
            : EntitySystem(world, priority)
            , _filter(filter)
        {
            
        }

        void FilterSystem::onComponentChange(EVENT_HANDLER(EntityManager))
        {
            EVENT_ATTRS(EntityManager, EntityEventArgs);

            auto entity = args->entity;

            auto contains = utils::IsFlagSet(GetTypeMask(), entity->_system_mask);
            auto interests = _filter.Matches(entity);
            auto removed = args->event_type == EM_COMPONENT_REMOVED;

            if (interests && !contains && !removed)
            {
                Add(entity);
            }
            else if (!interests && contains)
            {
                Remove(entity);
            }
            else if (entity->IsAvailable())
            {
                Enable(entity);
            }
            else
            {
                Disable(entity);
            }
        }

        void FilterSystem::onEntityRemoved(EVENT_HANDLER(EntityManager))
        {
            EVENT_ATTRS(EntityManager, EntityEventArgs);

            auto entity = args->entity;

            // remove the entity from this system if it interested us before
            // removal
            if (_filter.Matches(entity))
                Remove(entity);
        }

        void FilterSystem::onUpdate(EVENT_HANDLER(SystemManager))
        {
            Begin();

            for (auto entity : _active)
                Process(entity.second);

            End();
        }

        void FilterSystem::Add(Entity *entity)
        {
            entity->setSystem(GetTypeMask());

            if (entity->IsActive())
                Enable(entity);
        }

        void FilterSystem::Remove(Entity *entity)
        {
            entity->unsetSystem(GetTypeMask());

            Disable(entity);
        }

        void FilterSystem::Enable(Entity *entity)
        {
            auto id = entity->GetID();

            URSINE_TODO("Optimize with binary search");

            // already enabled
            if (_active.find(id) != _active.end())
                return;

            _active[id] = entity;
        }

        void FilterSystem::Disable(Entity *entity)
        {
            auto id = entity->GetID();

            // doesn't exist
            if (_active.find(id) == _active.end())
                return;

            _active.erase(id);
        }

        void FilterSystem::OnInitialize(void)
        {
            _world->Manager<EntityManager>()->Listener(this)
                .On(EM_COMPONENT_ADDED, &FilterSystem::onComponentChange)
                .On(EM_COMPONENT_REMOVED, &FilterSystem::onComponentChange)
                .On(EM_ENTITY_REMOVED, &FilterSystem::onEntityRemoved);

            _world->Manager<SystemManager>()->Listener(this)
                .On(SM_UPDATE, &FilterSystem::onUpdate);
        }

        void FilterSystem::OnRemove(void)
        {
            _world->Manager<EntityManager>()->Listener(this)
                .Off(EM_COMPONENT_ADDED, &FilterSystem::onComponentChange)
                .Off(EM_COMPONENT_REMOVED, &FilterSystem::onComponentChange)
                .Off(EM_ENTITY_REMOVED, &FilterSystem::onEntityRemoved);

            _world->Manager<SystemManager>()->Listener(this)
                .Off(SM_UPDATE, &FilterSystem::onUpdate);
        }
    }
}