/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SystemManager.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "SystemManager.h"

namespace ursine
{
    namespace ecs
    {
        SystemManager::SystemManager(World *world)
            : WorldManager( world )
        {
            URSINE_TODO( "construct entity systems here" );
        }

        SystemManager::~SystemManager(void)
        {
            auto sortedSystems( m_systems );

            // sort based on priority
            std::sort( sortedSystems.begin( ), sortedSystems.end( ), &EntitySystem::compare );

            // cleanup systems created from their factory constructor (in reverse order)
            for (auto it = sortedSystems.rbegin( ); it != sortedSystems.rend( ); ++it)
            {
                auto *system = *it;

                if (system)
                {
                    system->OnRemove( );

                    delete system;
                }
            }
        }

        void SystemManager::OnInitialize(void)
        {
            auto sortedSystems( m_systems );

            // sort based on priority
            std::sort( sortedSystems.begin( ), sortedSystems.end( ), &EntitySystem::compare );

            for (auto system : sortedSystems)
            {
                if (system)
                    system->OnInitialize( );
            }
        }
    }
}
