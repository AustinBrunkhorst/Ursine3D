#include "UrsinePrecompiled.h"

#include "UtilityManager.h"

#include "Entity.h"

#include "TimerManager.h"
#include "TweenManager.h"

namespace ursine
{
    namespace ecs
    {
        UtilityManager::UtilityManager(World *world)
            : WorldManager( world ) { }

        UtilityManager::~UtilityManager(void) { }

        LocalTimerManager &UtilityManager::GetTimers(Entity *entity)
        {
            auto search = m_timers.find( entity->m_uniqueID );

            if (search == m_timers.end( ))
            {
                // add it for the first time
                return m_timers.emplace(
                    entity->m_uniqueID, 
                    Application::Instance->GetCoreSystem<TimerManager>( )
                ).first->second;
            }

            return search->second;
        }

        LocalTweenManager &UtilityManager::GetTweens(Entity *entity)
        {
            auto search = m_tweens.find( entity->m_uniqueID );

            if (search == m_tweens.end( ))
            {
                // add it for the first time
                return m_tweens.emplace(
                    entity->m_uniqueID, 
                    Application::Instance->GetCoreSystem<TweenManager>( )
                ).first->second;
            }

            return search->second;
        }

        void UtilityManager::Clear(Entity *entity)
        {
            GetTimers( entity ).ClearAll( );
            GetTweens( entity ).ClearAll( );
        }
    }
}
