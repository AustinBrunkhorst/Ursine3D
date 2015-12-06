/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** UtilityManager.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "WorldManager.h"

#include "LocalTimerManager.h"
#include "LocalTweenManager.h"

#include "EntityConfig.h"

namespace ursine
{
    namespace ecs
    {
        class Entity;

        class UtilityManager final : public WorldManager
        {
            std::unordered_map<EntityUniqueID, LocalTimerManager> m_timers;
            std::unordered_map<EntityUniqueID, LocalTweenManager> m_tweens;

        public:
            UtilityManager(World *world);
            ~UtilityManager(void);

            LocalTimerManager &GetTimers(Entity *entity);
            LocalTweenManager &GetTweens(Entity *entity);

            // Clears all utilities from this entity
            void Clear(Entity *entity);
        };
    }
}