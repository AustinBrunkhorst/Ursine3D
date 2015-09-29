/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** FilterSystem.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "EntitySystem.h"

#include "EntityConfig.h"
#include "Filter.h"

#include "EntityManager.h"
#include "SystemManager.h"

#include "EntityProcessor.h"

namespace ursine
{
    namespace ecs
    {
        class FilterSystem 
            : public EntitySystem
            , public EntityProcessor
        {
            // entity's components changed
            void onComponentChange(EVENT_HANDLER(EntityManager));

            // entity removed
            void onEntityRemoved(EVENT_HANDLER(EntityManager));

            // game tick
            void onUpdate(EVENT_HANDLER(SystemManager));

            const Filter m_filter;

        protected:
            std::unordered_map<EntityID, Entity*> m_active;

            void Add(Entity *entity);
            void Remove(Entity *entity);

            void Enable(Entity *entity);
            void Disable(Entity *entity);

            virtual void OnInitialize(void);
            virtual void OnRemove(void);
        public:
            FilterSystem(World *world, const Filter &filter, SystemPriority priority = 0);
        };
    }
}
