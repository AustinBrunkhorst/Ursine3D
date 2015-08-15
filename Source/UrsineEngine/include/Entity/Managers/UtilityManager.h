#pragma once

#include "WorldManager.h"

namespace Ursine
{
    namespace ECS
    {
        class Entity;

        class UtilityManager final : public WorldManager
        {
            std::unordered_map<EntityUniqueID, LocalTimerManager> _timers;
            std::unordered_map<EntityUniqueID, LocalTweenManager> _tweens;

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