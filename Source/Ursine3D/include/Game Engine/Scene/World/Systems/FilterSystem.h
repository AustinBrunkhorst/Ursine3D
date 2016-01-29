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
        public:
            FilterSystem(
                World *world, 
                const Filter &filter, 
                EventHandlerPriority updatePriority = kDefaultEventHandlerPriority
            );

            // entity's components changed
            void onComponentChange(EVENT_HANDLER(World));

            // entity removed
            void onEntityRemoved(EVENT_HANDLER(World));

            // game tick
            virtual void onUpdate(EVENT_HANDLER(World));

            const Filter m_filter;

        protected:
            std::unordered_map<EntityUniqueID, Entity*> m_active;

            void Add(Entity *entity);
            void Remove(Entity *entity);

            virtual void Enable(Entity *entity);
            virtual void Disable(Entity *entity);

            virtual void OnInitialize(void) override;
            virtual void Initialize(void);
            virtual void OnRemove(void) override;

            // Setting the update type must happen before OnInitialize is called
            void SetUpdateType(WorldEventType updateType);
            WorldEventType GetUpdateType(void) const;

        private:
            EventHandlerPriority m_updatePriority;
            
            // The world event that this filter system subscribes to (Editor vs Playmode)
            WorldEventType m_updateType;

        } Meta(Enable, WhiteListMethods);
    }
}
