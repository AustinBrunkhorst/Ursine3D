/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SystemContainer.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "SystemContainer.h"

namespace ursine
{
    SystemContainer::SystemContainer(void)
        : m_paused( false ) { }

    SystemContainer::~SystemContainer(void)
    {
        ClearSystems( );
    }

    void SystemContainer::UpdateSystems(void)
    {
        auto size = m_systemQueue.size( );

        for (uint32 i = 0; i < size; ++i)
        {
            auto system = m_systemQueue[ i ];

            if (!m_paused || system->m_updateOnPause)
                system->Update( );
        }
    }

    void SystemContainer::ClearSystems(void)
    {
        // delete remaining systems in the reverse order they were added
        for (auto it = m_systemQueue.rbegin( ); it != m_systemQueue.rend( ); ++it)
            delete *it;

        m_systemQueue.clear( );
    }

    void SystemContainer::Pause(bool pause)
    {
        m_paused = pause;
    }
}
