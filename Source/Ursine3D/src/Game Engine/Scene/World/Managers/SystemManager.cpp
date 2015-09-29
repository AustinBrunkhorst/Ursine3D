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
            for (auto *system : m_systems)
                system->OnRemove( );

            for (auto *system : m_systems)
                delete system;
        }

        void SystemManager::OnInitialize(void)
        {
            for (auto system : m_systems)
                system->OnInitialize( );
        }
    }
}
