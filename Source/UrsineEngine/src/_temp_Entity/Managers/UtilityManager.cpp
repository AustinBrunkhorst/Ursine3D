#include "UrsinePrecompiled.h"

#include "UtilityManager.h"

namespace ursine
{
    namespace ecs
    {
        UtilityManager::UtilityManager(World *world)
            : WorldManager(world) { }

        UtilityManager::~UtilityManager(void) { }

        LocalTimerManager &UtilityManager::GetTimers(Entity *entity)
        {
            return _timers[entity->m_uniqueID];
        }

        LocalTweenManager &UtilityManager::GetTweens(Entity *entity)
        {
            return _tweens[entity->m_uniqueID];
        }

        void UtilityManager::Clear(Entity* entity)
        {
            GetTimers(entity).ClearAll();
            GetTweens(entity).ClearAll();
        }
    }
}
