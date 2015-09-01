/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SystemContainer.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "System.h"

namespace ursine
{
    class SystemContainer
    {
        bool m_paused;

        // systems added to this container
        std::unordered_map<size_t, System*> m_systems;

        // systems with the order in which they were added 
        std::vector<System *> m_systemQueue;

    public:
        SystemContainer(void);
        virtual ~SystemContainer(void);

        // Adds a system to this container
        template<class SystemType, typename... Args>
        SystemType *AddSystem(Args&&...args);

        // Updates all systems in this container
        void UpdateSystems(void);

        // Removes all systems
        void ClearSystems(void);

        void Pause(bool pause);
    };
}

#include "SystemContainer.hpp"