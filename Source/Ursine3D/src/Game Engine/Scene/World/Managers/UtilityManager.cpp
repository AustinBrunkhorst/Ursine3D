#include "UrsinePrecompiled.h"

#include "UtilityManager.h"

#include "Entity.h"

namespace ursine
{
    namespace ecs
    {
        UtilityManager::UtilityManager(World *world)
            : WorldManager(world) { }

        UtilityManager::~UtilityManager(void) { }

        LocalTimerManager &UtilityManager::GetTimers(Entity *entity)
        {
            return m_timers[ entity->m_uniqueID ];
        }

        LocalTweenManager &UtilityManager::GetTweens(Entity *entity)
        {
            return m_tweens[ entity->m_uniqueID ];
        }

        void UtilityManager::Clear(Entity *entity)
        {
            GetTimers( entity ).ClearAll( );
            GetTweens( entity ).ClearAll( );
        }
    }
}
