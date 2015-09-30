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
            void onComponentChange(EVENT_HANDLER(World));

            // entity removed
            void onEntityRemoved(EVENT_HANDLER(World));

            // game tick
            void onUpdate(EVENT_HANDLER(World));

            const Filter m_filter;

        protected:
            std::unordered_map<EntityUniqueID, Entity*> m_active;

            void Add(Entity *entity);
            void Remove(Entity *entity);

            void Enable(Entity *entity);
            void Disable(Entity *entity);

            virtual void OnInitialize(void);
            virtual void OnRemove(void);
        public:
            FilterSystem(World *world, const Filter &filter);
        };
    }
}
