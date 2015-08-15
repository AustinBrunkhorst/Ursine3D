#include "UrsinePrecompiled.h"

#include "UtilityManager.h"

namespace Ursine
{
    namespace ECS
    {
        UtilityManager::UtilityManager(World *world)
            : WorldManager(world) { }

        UtilityManager::~UtilityManager(void) { }

        LocalTimerManager &UtilityManager::GetTimers(Entity *entity)
        {
            return _timers[entity->_unique_id];
        }

        LocalTweenManager &UtilityManager::GetTweens(Entity *entity)
        {
            return _tweens[entity->_unique_id];
        }

        void UtilityManager::Clear(Entity* entity)
        {
            GetTimers(entity).ClearAll();
            GetTweens(entity).ClearAll();
        }
    }
}
